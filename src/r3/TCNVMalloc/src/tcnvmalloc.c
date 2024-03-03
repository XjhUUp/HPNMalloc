#include "tcnvmalloc.h"
#include "pthread.h"
#include <fcntl.h>
#include <sys/stat.h>

/* Global metadata */
pthread_once_t init_once = PTHREAD_ONCE_INIT;
thread_state_t global_state = UNINIT;
pthread_key_t destructor;
// gpool_t *gpool;
gpool_t *pool0;
gpool_t *pool1;
gpool_t *pool2;
gpool_t *pool3;
extern nvm_list_t nvm_list_area;
int nvm_index;

/* Mappings */
CACHE_ALIGN int cls2size[128];
char sizemap[256];
char sizemap2[128];

/* threads */
THREAD_LOCAL thread_state_t thread_state = UNINIT;
THREAD_LOCAL lheap_t *local_heap = NULL;

inline static void thread_init();
inline static void check_init();
inline static void global_init();
inline static void thread_exit();
inline static void gpool_init();
inline static void maps_init();
inline static void *small_malloc(int size_cls);
inline static void *large_malloc(size_t size);

/* global pool operation */
inline static void gpool_grow();
inline static chunkh_t *gpool_acquire_chunk();

/* helper */
inline static void *page_alloc(void *pos, size_t size);
inline static void page_free(void *pos, size_t size);
inline static lheap_t *acquire_lheap();
inline static void *chunk_alloc_obj(chunkh_t *ch);
inline static void chunk_init(chunkh_t *ch, int size_cls);
inline static chunkh_t *chunk_extract_header(void *ptr);
inline static void chunk_free_small(chunkh_t *ch, void *ptr);
inline static void chunk_free_large(chunkh_t *ch);
inline void lheap_replace_foreground(lheap_t *lh, int size_cls);

/* implementation */
inline static int chunk_comp(void *i, void *j)
{
    chunkh_t *chi = (chunkh_t *)i;
    chunkh_t *chj = (chunkh_t *)j;

    return (chi->wear_tot < chj->wear_tot) ? 1 : 0;
}

static void *reap_chunk(void *arg)
{
    int sleeptime = 1;
    chunkh_t *ch;
    int ret;

    while (1)
    {
        sleep(sleeptime);
        // for (int i = 0; i < nvm_list_area.nvm_index; i++)
        // {
        //     nvm_area_t *nvm = nvm_list_area.nvm[i];
        //     gpool_t *gpool = &nvm->cpool;
        //     pthread_mutex_lock(&gpool->lock);
        //     while (!list_empty(&gpool->free_list))
        //     {
        //         ch = list_entry(gpool->free_list.next, chunkh_t, list);

        //         ch->wear_tot++;
        //         ret = pq_insert(&gpool->pq, ch);
        //         if (ret != 0)
        //         {
        //             fprintf(stderr, "pq_insert error\n");
        //             exit(-1);
        //         }

        //         list_del(&ch->list);
        //     }

        //     pthread_mutex_unlock(&gpool->lock);
        // }

        // pthread_mutex_lock(&gpool->lock);
        // while (!list_empty(&gpool->free_list))
        // {
        //     ch = list_entry(gpool->free_list.next, chunkh_t, list);

        //     ch->wear_tot++;
        //     ret = pq_insert(&gpool->pq, ch);
        //     if (ret != 0)
        //     {
        //         fprintf(stderr, "pq_insert error\n");
        //         exit(-1);
        //     }

        //     list_del(&ch->list);
        // }

        // pthread_mutex_unlock(&gpool->lock);
    }

    return NULL;
}

static int open_empty_or_create_file(char *path)
{
    int fd = -1;
    struct stat stbuf;
    if (stat(path, &stbuf) < 0)
    {
        if ((fd = open(path, O_RDWR | O_CREAT, 0666)) < 0)
            fprintf(stderr, "unable to create file\n");
    }
    else if ((fd = open(path, O_RDWR | O_TRUNC)) < 0)
    {
        // O_TRUNC：不存在即报错，一般配合O_CREAT使用，已存在即清空
        fprintf(stderr, "unable to open file\n");
    }
    return fd;
}

void init_nvm_area_list()
{

    nvm_list_area.nvm_index = 0;
}

static void central_pool_init(gpool_t *central_pool, nvm_area_t *nvm_area, int flag)
{
    if (pthread_mutex_init(&central_pool->lock, NULL) < 0)
    {
        fprintf(stderr, "fatal error: pthread_mutex_init failed\n");
        exit(-1);
    }
    void *ret = NULL;
    int fd;

    if (flag == 0)
    {
        ret = RAW_POOL_START0;
        fd = open_empty_or_create_file(NVM_FILE_PATH0);
    }
    else if (flag == 1)
    {
        ret = RAW_POOL_START1;
        fd = open_empty_or_create_file(NVM_FILE_PATH1);
    }
    else if (flag == 2)
    {
        ret = RAW_POOL_START2;
        fd = open_empty_or_create_file(NVM_FILE_PATH2);
    }
    else if (flag == 3)
    {
        ret = RAW_POOL_START3;
        fd = open_empty_or_create_file(NVM_FILE_PATH3);
    }

    // ret = (void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);

    /*
        NVM部分
        1. 打开或创建文件
        2. 扩张文件
    */
    central_pool->next_nvm_start = 0;
    central_pool->fd = fd;
    if (posix_fallocate(fd, 0, APPLY_PHY_MEMORY) != 0)
    {
        fprintf(stderr, "nvm_alloc failed\n");
    }
    if (mmap(ret, APPLY_PHY_MEMORY, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, central_pool->next_nvm_start) == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed\n");
    }

    central_pool->next_nvm_start += APPLY_PHY_MEMORY;

    central_pool->pool_start = (void *)(((uint64_t)ret + CHUNK_SIZE - 1) / CHUNK_SIZE * CHUNK_SIZE);
    ;
    central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    central_pool->free_start = central_pool->pool_start;
    central_pool->free_end = central_pool->pool_end;

    INIT_LIST_HEAD(&central_pool->free_list);
    pq_init(&central_pool->pq, chunk_comp, PQ_DEFAULT_SIZE);
    central_pool->nvm_area = nvm_area;
}

static void init_nvm()
{
    // NVM参数：1GB内存、磨损度10的12（STTRAM）
    nvm_area_t *n_area0 = make_init_nvm_area(1024.0 * 1024 * 1024, 0, 1000000000000, 1000000000000 - 1, 4.51, 1);
    pool0 = &n_area0->cpool;
    central_pool_init(&n_area0->cpool, n_area0, 0);
    // NVM参数：1GB内存 磨损度10的12（STTRAM）
    nvm_area_t *n_area1 = make_init_nvm_area(1024.0 * 1024 * 1024, 0, 1000000000000, 1000000000000 - 1, 4.51, 1);
    pool1 = &n_area1->cpool;
    central_pool_init(&n_area1->cpool, n_area1, 1);
    // NVM参数：2GB内存 磨损度10的8（PCM）
    nvm_area_t *n_area2 = make_init_nvm_area(2.0 * 1024 * 1024 * 1024,0, 100000000, 100000000 - 1, 4.51, 1);
    pool2 = &n_area2->cpool;
    central_pool_init(&n_area2->cpool, n_area2, 2);
    // NVM参数：3GB内存，磨损度10的10次方（RERAM）
    nvm_area_t *n_area3 = make_init_nvm_area(3.0 * 1024 * 1024 * 1024,0, 10000000000, 10000000000 - 1, 4.51, 1);
    pool3 = &n_area3->cpool;
    central_pool_init(&n_area3->cpool, n_area3, 3);
}

static void gpool_init()
{
    init_nvm_area_list();
    init_nvm();
    /* start thread to reap chunk asynchronously */
    pthread_t pid;
    if (pthread_create(&pid, NULL, reap_chunk, NULL) < 0)
    {
        fprintf(stderr, "pthread_create error\n");
        exit(-1);
    }
    // gpool = pool0;
    nvm_index = 0;
}

static void maps_init()
{
    int size;
    int class;

    for (size = 16, class = 0; size <= 256; size += 8, class ++)
    {
        cls2size[class] = size;
    }

    for (size = 256; size < 65536; size <<= 1)
    {
        cls2size[class ++] = size + (size >> 1);
        cls2size[class ++] = size << 1;
    }

    int cur_class = 0;
    int cur_size = 0;

    for (cur_size = 8; cur_size <= 1024; cur_size += 8)
    {
        if (cur_size > cls2size[cur_class])
            cur_class++;
        sizemap[(cur_size - 1) >> 3] = cur_class;
    }

    /* init sizemap2 */
    for (cur_size = 1024; cur_size <= 65536; cur_size += 512)
    {
        if (cur_size > cls2size[cur_class])
            cur_class++;
        sizemap2[(cur_size - 1) >> 9] = cur_class;
    }
}

static void thread_exit()
{
    // TODO: reclaim the memory space in local_heap
    // printf("exit\n");
    free(local_heap);
}

void cleanup()
{
    // printf("central pool free start:%p\n",gpool.free_start);
    // printf("main exit\n");
}

static void global_init()
{
    pthread_key_create(&destructor, thread_exit);
    gpool_init();
    maps_init();
    global_state = INITED;
    atexit(cleanup);
}

static void check_init()
{
    if (unlikely(thread_state != INITED))
    {
        if (unlikely(global_state != INITED))
        {
            pthread_once(&init_once, global_init);
        }
        thread_init();
    }
}

static void thread_init()
{
    /* make thread_exit executed when thread quit */
    pthread_setspecific(destructor, (void *)1);

    local_heap = acquire_lheap();
    thread_state = INITED;
}

static void *chunk_alloc_obj(chunkh_t *ch)
{
    void *ret = NULL;

    if (ch->free_mem_cnt > 0)
    {
        ret = ch->free_mem;
        ch->free_mem += ch->blk_size;
        ch->free_mem_cnt--;
    }
    else if (!dlist_empty(&ch->dlist_head, &ch->dlist_tail))
    {
        ret = (void *)ch->dlist_tail.prev;
        dlist_remove((dlist_t *)ret);
    }

    return ret;
}

static void chunk_init(chunkh_t *ch, int size_cls)
{
    ch->size_cls = size_cls;
    ch->blk_size = cls2size[size_cls];
    ch->blk_cnt = (CHUNK_DATA_SIZE) / ch->blk_size;
    ch->free_tot_cnt = ch->free_mem_cnt = ch->blk_cnt;
    ch->free_mem = (void *)ch + sizeof(chunkh_t);

    dlist_init(&ch->dlist_head, &ch->dlist_tail);

    ch->wear_count = 0;
}

static chunkh_t *chunk_extract_header(void *ptr)
{
    return (chunkh_t *)((uint64_t)ptr - (uint64_t)(ptr) % CHUNK_SIZE);
}

void lheap_replace_foreground(lheap_t *lh, int size_cls)
{
    chunkh_t *ch;

    if (!list_empty(&(lh->background[size_cls])))
    {
        ch = list_entry(lh->background[size_cls].next, chunkh_t, list);
        list_del(&ch->list);
        goto finish;
    }

    // TODO: allocate from free list
    if (!list_empty(&lh->free_list))
    {
        ch = list_entry(lh->free_list.next, chunkh_t, list);
        list_del(&ch->list);
        goto finish;
    }

    /* get chunk from gpool */

    int cur_index = 0;
    while (1)
    {
        // printf("%d\n", cur_index);
        nvm_area_t *nvm_area = nvm_list_area.nvm[cur_index];
        gpool_t *gpool = &nvm_area->cpool;
        pthread_mutex_lock(&gpool->lock);
        if (nvm_area->hp_table.used_mc + APPLY_PHY_MEMORY < nvm_area->hp_table.hardware_mc)
        {
            ch = gpool_acquire_chunk(gpool);
            pthread_mutex_unlock(&gpool->lock);
            break;
        }
        pthread_mutex_unlock(&gpool->lock);
        cur_index++;
    }

    // ch = gpool_acquire_chunk();

    check(ch != NULL, "gpool_acquire_chunk");

    ch->owner = lh;
    chunk_init(ch, size_cls);

finish:
    lh->foreground[size_cls] = ch;
    ch->state = FORG;
}

static void chunk_free_small(chunkh_t *ch, void *ptr)
{
    gpool_t *pool;
    if (ptr >= RAW_POOL_START3)
    {
        pool = pool3;
    }
    else if (ptr >= RAW_POOL_START2)
    {
        pool = pool2;
    }
    else if (ptr >= RAW_POOL_START1)
    {
        pool = pool1;
    }
    else
    {
        pool = pool0;
    }
    // TODO: race condition when remote free
    dlist_add(&ch->dlist_head, (dlist_t *)ptr);
    ch->free_tot_cnt++;

    int size_cls = ch->size_cls;
    lheap_t *lh = ch->owner;

    switch (ch->state)
    {
    case FORG:
        /* do nothing */
        break;

    case FULL:
        list_add_tail(&ch->list, &lh->background[size_cls]);
        ch->state = BACK;
        break;

    case BACK:
        if (unlikely(ch->free_tot_cnt == ch->blk_cnt))
        {
            list_del(&ch->list);
            list_add_tail(&ch->list, &lh->free_list);
        }
        break;

    case NAVA:
        if (unlikely(ch->free_tot_cnt == ch->blk_cnt))
        {
            pthread_mutex_lock(&pool->lock);
            list_add_tail(&ch->list, &pool->free_list);
            pool->nvm_area->hp_table.used_mc -= CHUNK_DATA_SIZE;
            pthread_mutex_unlock(&pool->lock);
        }
        break;

    default:
        fprintf(stderr, "fatal error: unknown state: %d\n", ch->state);
        exit(-1);
    }
}

static void chunk_free_large(chunkh_t *ch)
{
    page_free(ch->free_mem, ch->blk_size);
}

static void *small_malloc(int size_cls)
{
    lheap_t *lh = local_heap;
    chunkh_t *ch;
    void *ret;
retry:
    ch = lh->foreground[size_cls];
    ret = chunk_alloc_obj(ch);

    if (unlikely(--ch->free_tot_cnt == 0))
    {
        ch->state = FULL;
        lheap_replace_foreground(lh, size_cls);

        if (unlikely(ch->size_cls == DUMMY_CLASS))
        {
            ch->free_tot_cnt = 1;
            goto retry;
        }
    }

    ch->wear_count++;
    /* TODO: replace WEAR_LIMIT by blk_size related? */
    if (unlikely(ch->wear_count) >= WEAR_LIMIT)
    {
        ch->state = NAVA;
        lheap_replace_foreground(lh, size_cls);
    }

    return ret;
}

static void *large_malloc(size_t size)
{
    size_t size_tot = size + sizeof(chunkh_t) + CHUNK_SIZE;

    void *start_mem = page_alloc(NULL, size_tot);
    void *ret = (void *)ROUNDUP((uint64_t)start_mem, CHUNK_SIZE);
    chunkh_t *ch = (chunkh_t *)ret;
    ch->owner = (lheap_t *)LARGE_OWNER;
    ch->free_mem = start_mem;
    ch->blk_size = size_tot;

    return ret + sizeof(chunkh_t);
}

static void gpool_grow(gpool_t *gpool)
{

    // void *ret = page_alloc(gpool->pool_end, ALLOC_UNIT);
    // if (ret < 0) {
    //     fprintf(stderr, "page_alloc failed\n");
    //     exit(-1);
    // }

    // gpool->pool_end += ALLOC_UNIT;
    int fd = gpool->fd;
    void *ret = gpool->pool_end;
    int pos = gpool->next_nvm_start;
    if (posix_fallocate(fd, pos, APPLY_PHY_MEMORY) != 0)
    {
        fprintf(stderr, "nvm_alloc failed\n");
    }
    if (mmap(ret, APPLY_PHY_MEMORY, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, pos) == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed\n");
    }

    gpool->free_start = (void *)(((uint64_t)ret + CHUNK_SIZE - 1) / CHUNK_SIZE * CHUNK_SIZE);
    gpool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    gpool->free_end = gpool->pool_end;
}

inline static chunkh_t *gpool_acquire_chunk(gpool_t *gpool)
{
    chunkh_t *ch = gpool->free_start;
    do
    {
        if (gpool->free_start + CHUNK_SIZE <= gpool->pool_end)
        {
            gpool->free_start += CHUNK_SIZE;
            ch->wear_tot = 0;
            break;
        }

        if (!pq_is_empty(&gpool->pq))
        {
            ch = (chunkh_t *)pq_min(&gpool->pq);
            if (pq_delmin(&gpool->pq) < 0)
            {
                fprintf(stderr, "pq_delmin error\n");
                exit(-1);
            }

            break;
        }

        gpool_grow(gpool);
        gpool->free_start += CHUNK_SIZE;
    } while (0);
    gpool->nvm_area->hp_table.used_mc += CHUNK_DATA_SIZE;

    return ch;
}

inline static int size2cls(size_t size)
{
    int ret;
    if (likely(size <= 1024))
    {
        ret = sizemap[(size - 1) >> 3];
    }
    else if (size <= 65536)
    {
        ret = sizemap2[(size - 1) >> 9];
    }
    else
    {
        ret = LARGE_CLASS;
    }

    return ret;
}

void *wa_malloc(size_t size)
{
    // printf("RAW_POOL_START：%p header size:%d\n",gpool.pool_start,sizeof(chunkh_t));
    void *ret = NULL;
    check_init();

    size += (size == 0);

    int size_cls = size2cls(size);
    if (likely(size_cls < DEFAULT_BLOCK_CLASS))
    {
        ret = small_malloc(size_cls);
    }
    else if (size_cls == LARGE_CLASS)
    {
        ret = large_malloc(size);
    }
    else
    {
        fprintf(stderr, "fatal error: unknown class %d\n", size_cls);
        exit(-1);
    }

    return ret;
}

void *wa_realloc(void *ptr, size_t size)
{
    return NULL;
}

void wa_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    chunkh_t *ch = chunk_extract_header(ptr);
    lheap_t *lh = local_heap;
    lheap_t *target_lh = ch->owner;

    if (likely(target_lh == lh || (uint64_t)target_lh != LARGE_OWNER))
    {
        chunk_free_small(ch, ptr);
    }
    else
    {
        chunk_free_large(ch);
    }
}

static void *page_alloc(void *pos, size_t size)
{
    return mmap(pos, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

static void page_free(void *pos, size_t size)
{
    munmap(pos, size);
}

static lheap_t *acquire_lheap()
{
    lheap_t *lh = (lheap_t *)malloc(sizeof(lheap_t));
    if (!lh)
    {
        fprintf(stderr, "in acquire_lheap: malloc failed\n");
        return NULL;
    }

    int i;
    for (i = 0; i < DEFAULT_BLOCK_CLASS; i++)
    {
        lh->foreground[i] = &(lh->dummy_chunk);
        INIT_LIST_HEAD(&(lh->background[i]));
    }
    INIT_LIST_HEAD(&lh->free_list);

    lh->dummy_chunk.owner = lh;
    lh->dummy_chunk.size_cls = DUMMY_CLASS;
    lh->dummy_chunk.blk_size = 0;
    lh->dummy_chunk.blk_cnt = 0;
    lh->dummy_chunk.free_mem_cnt = 0;
    lh->dummy_chunk.free_tot_cnt = 1;
    lh->dummy_chunk.free_mem = (void *)0;
    dlist_init(&lh->dummy_chunk.dlist_head, &lh->dummy_chunk.dlist_tail);

    return lh;
}
