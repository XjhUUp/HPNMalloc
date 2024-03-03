#include "pthread.h"
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hpnmalloc1.h"

pthread_once_t init_once = PTHREAD_ONCE_INIT;
thread_state_t global_state = UNINIT;
pthread_key_t destructor;
THREAD_LOCAL thread_state_t thread_state = UNINIT;
THREAD_LOCAL local_cache_t *local_cache = NULL;

central_pool_t *pool0;
central_pool_t *pool1;
central_pool_t *pool2;
central_pool_t *pool3;

extern nvm_list_t nvm_list_area;

int bitmap_auxiliary_array[32] = {
    1, 2, 29, 3, 30, 15, 25, 4, 31, 23, 21, 16, 26, 18, 5, 9,
    32, 28, 14, 24, 22, 20, 17, 8, 27, 13, 19, 7, 12, 6, 11, 10};

#define LOW1BIT(X) (bitmap_auxiliary_array[((unsigned int)(X * 0x077CB531U)) >> 27])

static void *page_alloc(void *pos, size_t size)
{
    return mmap(pos, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

static span_t *span_extract_header(void *ptr)
{
    central_pool_t *p;
    void *pool_start = NULL;
    void *span_map = NULL;
    if (ptr >= RAW_POOL_START3)
    {
        pool_start = RAW_POOL_START3;
        span_map = SPAN_MAP_POOL_START3;
        p = pool3;
    }
    else if (ptr >= RAW_POOL_START2)
    {
        pool_start = RAW_POOL_START2;
        span_map = SPAN_MAP_POOL_START2;
        p = pool2;
    }
    else if (ptr >= RAW_POOL_START1)
    {
        pool_start = RAW_POOL_START1;
        span_map = SPAN_MAP_POOL_START1;
        p = pool1;
    }
    else
    {
        pool_start = RAW_POOL_START0;
        span_map = SPAN_MAP_POOL_START0;
        p = pool0;
    }

    // 计算是哪一段内存
    int memory_index = (ptr - pool_start) / (APPLY_VIRTUAL_MEMORY);
    // 计算是该段内存的哪一个索引
    int span_index = (ptr - pool_start) / PAGE_SIZE;
    span_index = span_index % (APPLY_PHY_MEMORY / PAGE_SIZE);
    // 获取对应的地址段的首地址
    void *addr = p->span_map[memory_index];
    // 获取存放span地址的地址
    addr = addr + span_index * 8;
    // 取出地址值
    span_t *span = *(uint64_t *)addr;
    return span;
}

static void span_back_to_centralpool(span_t *span)
{
    central_pool_t *central_pool = span->pool;
    int index;
    // span->flag = false;
    if (span->span_category != 256)
    {
        span->sl_block_t.page_num = 1;
    }
    // span = merge_span(span);
    int page_num = span->sl_block_t.page_num;
    if (page_num <= 256)
    {
        index = page_num - 1;
    }
    else
    {
        index = 256;
    }
    list_add_tail(&span->span_free_list, &central_pool->lb_free_list[index]);
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

static void central_pool_init(central_pool_t *central_pool, nvm_area_t *nvm_area, int flag)
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

    ret = (void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);

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

    central_pool->pool_start = ret;
    central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    central_pool->free_start = central_pool->pool_start;
    central_pool->free_end = central_pool->pool_end;
    for (int i = 0; i < LARGE_BLOCK_NUM; i++)
    {
        INIT_LIST_HEAD(&central_pool->lb_free_list[i]);
    }
    central_pool->nvm_area = nvm_area;

    central_pool->wear_limit = 0;

    central_pool->wear_count=0;
}

static void init_head_pool(central_pool_t *pool, int flag)
{
    void *next_span_request_addr = NULL;
    void *span_head_pool_start;
    if (flag == 0)
    {
        next_span_request_addr = SPAN_HEAD_POOL_START0;
        span_head_pool_start = SPAN_HEAD_POOL_START0;
    }
    else if (flag == 1)
    {
        next_span_request_addr = SPAN_HEAD_POOL_START1;
        span_head_pool_start = SPAN_HEAD_POOL_START1;
    }
    else if (flag == 2)
    {
        next_span_request_addr = SPAN_HEAD_POOL_START2;
        span_head_pool_start = SPAN_HEAD_POOL_START2;
    }
    else
    {
        next_span_request_addr = SPAN_HEAD_POOL_START3;
        span_head_pool_start = SPAN_HEAD_POOL_START3;
    }
    pool->span_head_pool_start = span_head_pool_start;
    pool->next_request_addr = next_span_request_addr;
    void *span_head_addr = page_alloc(next_span_request_addr, APPLY_SPAN_HEAD_MEMORY);
    if (span_head_addr < 0)
    {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    pool->next_request_addr = span_head_addr + APPLY_SPAN_HEAD_MEMORY;
}

static void init_span_map(central_pool_t *pool, int flag)
{
    void *next_span_map_addr = NULL;
    if (flag == 0)
    {
        next_span_map_addr = SPAN_MAP_POOL_START0;
    }
    else if (flag == 1)
    {
        next_span_map_addr = SPAN_MAP_POOL_START1;
    }
    else if (flag == 2)
    {
        next_span_map_addr = SPAN_MAP_POOL_START2;
    }
    else
    {
        next_span_map_addr = SPAN_MAP_POOL_START3;
    }
    pool->next_span_map_addr = next_span_map_addr;
    void *addr = page_alloc(next_span_map_addr, APPLY_SPAN_MAP_MEMORY);
    if (addr < 0)
    {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    pool->span_map[0] = addr;
    pool->next_span_map_addr = addr + APPLY_SPAN_MAP_MEMORY;
}

static void init_nvm()
{
    // NVM参数：1GB内存、磨损度10的12（STTRAM）
    nvm_area_t *n_area0 = make_init_nvm_area(1024.0 * 1024 * 1024, 0, 1000000000000, 1000000000000 - 1, 4.51, 1);
    pool0 = &n_area0->cpool;
    central_pool_init(&n_area0->cpool, n_area0, 0);
    init_head_pool(&n_area0->cpool, 0);
    init_span_map(&n_area0->cpool, 0);
    // NVM参数：1GB内存 磨损度10的12（STTRAM）
    nvm_area_t *n_area1 = make_init_nvm_area(1024.0 * 1024 * 1024, 0, 1000000000000, 1000000000000 - 1, 4.51, 1);
    pool1 = &n_area1->cpool;
    central_pool_init(&n_area1->cpool, n_area1, 1);
    init_head_pool(&n_area1->cpool, 1);
    init_span_map(&n_area1->cpool, 1);
    // NVM参数：2GB内存 磨损度10的8（PCM）
    nvm_area_t *n_area2 = make_init_nvm_area(2.0 * 1024 * 1024 * 1024, 0, 100000000, 100000000 - 1, 4.51, 1);
    pool2 = &n_area2->cpool;
    central_pool_init(&n_area2->cpool, n_area2, 2);
    init_head_pool(&n_area2->cpool, 2);
    init_span_map(&n_area2->cpool, 2);
    // NVM参数：3GB内存，磨损度10的10次方（RERAM）
    nvm_area_t *n_area3 = make_init_nvm_area(3.0 * 1024 * 1024 * 1024, 0, 10000000000, 10000000000 - 1, 4.51, 1);
    pool3 = &n_area3->cpool;
    central_pool_init(&n_area3->cpool, n_area3, 3);
    init_head_pool(&n_area3->cpool, 3);
    init_span_map(&n_area3->cpool, 3);
}

static void global_init()
{
    init_nvm_area_list();
    init_nvm();
    global_state = INITED;
}

// static void thread_exit()
// {
//     span_t *span = NULL;
//     for (int i = 0; i < SMALL_BLOCK_NUM; i++)
//     {
//         if (local_cache->foreground[i] != NULL)
//         {
//             span = local_cache->foreground[i];
//             list_del(&span->span_free_list);
//             pthread_mutex_lock(&span->pool->nvm_area->lock);
//             span_back_to_centralpool(span);
//             pthread_mutex_unlock(&span->pool->nvm_area->lock);
//         }
//     }
//     for (int i = 0; i < SMALL_BLOCK_NUM; i++)
//     {
//         while (!list_empty(&local_cache->background[i]))
//         {
//             span = local_cache->background[i].next;
//             list_del(&span->span_free_list);
//             pthread_mutex_lock(&span->pool->nvm_area->lock);
//             span_back_to_centralpool(span);
//              pthread_mutex_unlock(&span->pool->nvm_area->lock);
//         }
//     }
//     while (!list_empty(&local_cache->free_list))
//     {
//         span = local_cache->free_list.next;
//         list_del(&span->span_free_list);
//         pthread_mutex_lock(&span->pool->nvm_area->lock);
//         span_back_to_centralpool(span);
//         pthread_mutex_lock(&span->pool->nvm_area->lock);
//     }

// }

static local_cache_t *init_local_cache()
{
    local_cache_t *local_cache = (local_cache_t *)malloc(sizeof(local_cache_t));
    for (int i = 0; i < SMALL_BLOCK_NUM; i++)
    {
        local_cache->foreground[i] = NULL;
        INIT_LIST_HEAD(&(local_cache->background[i]));
    }
    INIT_LIST_HEAD(&local_cache->free_list);
    return local_cache;
}

static void thread_init()
{
    // pthread_setspecific(destructor, thread_exit);
    local_cache = init_local_cache();
    thread_state = INITED;
}

static void check_init()
{
    if (thread_state != INITED)
    {
        if (global_state != INITED)
        {
            pthread_once(&init_once, global_init);
        }
        thread_init();
    }
}

static void central_pool_grow(central_pool_t *central_pool)
{
    int fd = central_pool->fd;
    void *ret = central_pool->pool_end;
    int pos = central_pool->next_nvm_start;
    if (posix_fallocate(fd, pos, APPLY_PHY_MEMORY) != 0)
    {
        fprintf(stderr, "nvm_alloc failed\n");
    }
    if (mmap(ret, APPLY_PHY_MEMORY, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, pos) == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed\n");
    }
    void *span_head_addr = page_alloc(central_pool->next_request_addr, APPLY_SPAN_HEAD_MEMORY);
    if (span_head_addr < 0)
    {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    central_pool->next_request_addr = span_head_addr + APPLY_SPAN_HEAD_MEMORY;
    central_pool->free_start = (void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);
    central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    central_pool->free_end = central_pool->pool_end;
    // 扩展span_map
    int memory_index = (ret - central_pool->pool_start) / (APPLY_VIRTUAL_MEMORY);
    void *span_map_addr = page_alloc(central_pool->next_span_map_addr, APPLY_SPAN_MAP_MEMORY);
    if (span_map_addr < 0)
    {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }

    central_pool->next_span_map_addr = span_map_addr + APPLY_SPAN_MAP_MEMORY;
    // 计算是哪一段内存
    central_pool->span_map[memory_index] = span_map_addr;
}

static void reset_span_map(span_t *span)
{
    void *span_addr = span;
    // 计算是哪一段内存
    void *pool_start = span->pool->pool_start;
    size_t memory_index = (span->free_start - pool_start) / (APPLY_VIRTUAL_MEMORY);

    // 计算是该段内存的哪一个索引
    int span_index = (span->free_start - pool_start) / PAGE_SIZE;
    span_index = span_index % (APPLY_PHY_MEMORY / PAGE_SIZE);

    // 获取对应内存地址段的首地址
    void *addr = span->pool->span_map[memory_index];
    // 获取存放span地址的指针
    addr = addr + span_index * 8;
    // 获取要存放的span的64位地址值
    uint64_t s_addr = (uint64_t)span;
    // 将64位地址值保存
    *(uint64_t *)addr = s_addr;
}

static void span_init(span_t *span, int index, uint16_t span_category, void *free_start, bool flag, bool span_map_flag)
{
    if (span_category != 256)
    {
        span->span_category = index;
        span->sl_block_t.sm_b.total_block_num = PAGE_SIZE / ((index + 1) * 64);
        span->sl_block_t.sm_b.free_block_num = span->sl_block_t.sm_b.total_block_num;
        span->sl_block_t.sm_b.avaliable_free_start = free_start;
        INIT_LIST_HEAD(&span->sl_block_t.sm_b.block_free_list);
    }
    else
    {
        span->span_category = 256;
        span->sl_block_t.page_num = index + 1;
    }
    span->free_start = free_start;
    // span->flag = flag;
    INIT_LIST_HEAD(&span->span_free_list);
    if (span_map_flag == true)
    {
        reset_span_map(span);
    }
}

static span_t *span_split(list_head *list_node, int cur_index, int target_index, central_pool_t *central_pool)
{
    span_t *span = list_entry(list_node, span_t, span_free_list);
    if (cur_index == 256)
    {
        cur_index = span->sl_block_t.page_num - 1;
    }
    span_t *span_s = (void *)span + (target_index + 1) * SPAN_SIZE;
    int list_index_s = cur_index - target_index - 1;

    span_s->pool = central_pool;
    span_init(span_s, list_index_s, 256, span->free_start + (target_index + 1) * PAGE_SIZE, false, true);

    span_back_to_centralpool(span_s);

    // if (list_index_s > 255)
    // {
    //     list_add(&span_s->span_free_list, &central_pool->lb_free_list[LARGE_BLOCK_NUM - 1]);
    // }
    // else
    // {
    //     list_add(&span_s->span_free_list, &central_pool->lb_free_list[list_index_s]);
    // }
    return span;
}

void init_span_pool(span_t *span, central_pool_t *pool)
{
    span->pool = pool;
}

static span_t *acquire_span(central_pool_t *central_pool, int index)
{
    span_t *span = NULL;
    while (1)
    {
        if (central_pool->free_start + (index + 1) * PAGE_SIZE < central_pool->free_end)
        {
            goto finsh;
            break;
        }
        else
        {
            if (!list_empty(&central_pool->lb_free_list[index]))
            {
                span = list_entry(central_pool->lb_free_list[index].next, span_t, span_free_list);
                list_del(&span->span_free_list);
                return span;
            }
            else
            {
                for (int i = index + 1; i < LARGE_BLOCK_NUM; i++)
                {
                    if (!list_empty(&central_pool->lb_free_list[i]))
                    {
                        if (i == LARGE_BLOCK_NUM - 1)
                        {
                            span = span_split(central_pool->lb_free_list[i].next, 256, index, central_pool);
                        }
                        else
                        {
                            span = span_split(central_pool->lb_free_list[i].next, i, index, central_pool);
                        }
                        list_del(&span->span_free_list);
                        return span;
                    }
                }
            }
        }
        central_pool_grow(central_pool);
        int span_index;
    finsh:
        span_index = (central_pool->free_start - central_pool->pool_start) / PAGE_SIZE;
        span = central_pool->span_head_pool_start + (span_index * SPAN_SIZE);
        span->free_start = central_pool->free_start;
        central_pool->free_start += PAGE_SIZE * (index + 1);
        init_span_pool(span, central_pool);
        break;
    }
    return span;
}

span_t *acquire_span_from_centralpool(central_pool_t *cpool, int index)
{
    span_t *span = NULL;
    span = acquire_span(cpool, 0);
    span_init(span, index, index, span->free_start, true, true);
    span_t *span_s = NULL;
    for (int i = 0; i < PAGE_FROM_POOL_NUM; i++)
    {

        span_s = acquire_span(cpool, 0);
        list_add_tail(&span_s->span_free_list, &local_cache->free_list);
    }
    return span;
}

span_t *acquire_span_from_centralpool1(central_pool_t *cpool, int index)
{
    span_t *span = NULL;
    span = acquire_span(cpool, index);
    span_init(span, index, 256, span->free_start, true, true);
    return span;
}

static void recalculate_priority(central_pool_t *pool)
{
    nvm_area_t *narea = pool->nvm_area;
    //double w = (double)1 - (double)narea->cpool.wear_count / (double)narea->hp_table.memory_wear_limit;
    // if (narea->hp_table.wear_weight - w >= 0.1)
    // {
    //     narea->hp_table.wear_weight = narea->hp_table.wear_weight - 0.1;
    // }
    narea->hp_table.memory_wear_remain = narea->hp_table.memory_wear_limit - narea->cpool.wear_count;
    init_priority(narea);
}

span_t *acquire_span_from_localcache(int index)
{
    local_cache_t *lc = local_cache;
    span_t *span = NULL;
    if (lc->foreground[index] != NULL)
    {
        span = lc->foreground[index];
    }
    else if (!list_empty(&(lc->background[index])))
    {
        span = list_entry(lc->background[index].next, span_t, span_free_list);
        list_del(&span->span_free_list);
    }
    else if (!list_empty(&(lc->free_list)))
    {
        span = list_entry(lc->free_list.next, span_t, span_free_list);
        list_del(&span->span_free_list);
        span_init(span, index, index, span->free_start, true, true);
    }
    return span;
}

void *allocate_block(span_t *span)
{
    void *addr = NULL;
    int index = span->span_category;
    if (span->sl_block_t.sm_b.avaliable_free_start + (index + 1) * MIN_STORE_UNIT <= span->free_start + PAGE_SIZE)
    {
        addr = span->sl_block_t.sm_b.avaliable_free_start;
        span->sl_block_t.sm_b.avaliable_free_start += (index + 1) * MIN_STORE_UNIT;
        return addr;
    }
    else
    {
        addr = span->sl_block_t.sm_b.block_free_list.next;
        list_del(addr);
    }
    return addr;
}

static void *small_malloc(size_t size)
{
    void *addr = NULL;
    local_cache_t *lc = local_cache;
    span_t *span = NULL;
    int index = (size - 1) / MIN_STORE_UNIT;
    int nvm_index = nvm_list_area.nvm_index - 1;
    while (addr == NULL)
    {
        span = acquire_span_from_localcache(index);
        if (span == NULL)
        {
            nvm_area_t *nvm[10];
            nvm[0] = nvm_list_area.nvm[0];
            nvm[1] = nvm_list_area.nvm[1];
            nvm[2] = nvm_list_area.nvm[2];
            nvm[3] = nvm_list_area.nvm[3];
            nvm_area_t *temp;
            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }

            if (nvm[1]->priority > nvm[2]->priority)
            {
                temp = nvm[1];
                nvm[1] = nvm[2];
                nvm[2] = temp;
            }

            if (nvm[2]->priority > nvm[3]->priority)
            {
                temp = nvm[2];
                nvm[2] = nvm[3];
                nvm[3] = temp;
            }

            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }

            if (nvm[1]->priority > nvm[2]->priority)
            {
                temp = nvm[1];
                nvm[1] = nvm[2];
                nvm[2] = temp;
            }

            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }
            while (true)
            {
                if (!pthread_mutex_trylock(&nvm[nvm_index]->lock))
                {
                    nvm_area_t *nvm_area = nvm[nvm_index];
                    central_pool_t *cpool = &nvm_area->cpool;
                    span = acquire_span_from_centralpool(cpool, index);
                    pthread_mutex_unlock(&nvm_area->lock);
                    break;
                }
                else
                {
                    if (nvm_index - 1 == -1)
                    {
                        nvm_index = nvm_list_area.nvm_index - 1;
                    }
                    else
                    {
                        nvm_index -= 1;
                    }
                }
            }
        }
        lc->foreground[index] = span;
        addr = allocate_block(span);
        if (addr == NULL)
        {
            lc->foreground[index] = NULL;
        }
    }
    span->sl_block_t.sm_b.free_block_num--;
    if (span->sl_block_t.sm_b.free_block_num == 0)
    {
        lc->foreground[index] = NULL;
    }
    nvm_area_t *nvm_area = span->pool->nvm_area;
    central_pool_t *cpool = &nvm_area->cpool;
    pthread_mutex_lock(&nvm_area->lock);
    cpool->wear_count += 1;
    recalculate_priority(cpool);
    pthread_mutex_unlock(&nvm_area->lock);

    return addr;
}

static void *large_malloc(size_t size)
{
    void *addr = NULL;
    span_t *span = NULL;
    int index = ((size - 1) / PAGE_SIZE)+1;
    int nvm_index = nvm_list_area.nvm_index - 1;
    while (addr == NULL)
    {
        if (span == NULL)
        {
            nvm_area_t *nvm[10];
            nvm[0] = nvm_list_area.nvm[0];
            nvm[1] = nvm_list_area.nvm[1];
            nvm[2] = nvm_list_area.nvm[2];
            nvm[3] = nvm_list_area.nvm[3];
            nvm_area_t *temp;
            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }

            if (nvm[1]->priority > nvm[2]->priority)
            {
                temp = nvm[1];
                nvm[1] = nvm[2];
                nvm[2] = temp;
            }

            if (nvm[2]->priority > nvm[3]->priority)
            {
                temp = nvm[2];
                nvm[2] = nvm[3];
                nvm[3] = temp;
            }

            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }

            if (nvm[1]->priority > nvm[2]->priority)
            {
                temp = nvm[1];
                nvm[1] = nvm[2];
                nvm[2] = temp;
            }

            if (nvm[0]->priority > nvm[1]->priority)
            {
                temp = nvm[0];
                nvm[0] = nvm[1];
                nvm[1] = temp;
            }
            while (true)
            {
                if (!pthread_mutex_trylock(&nvm[nvm_index]->lock))
                {
                    nvm_area_t *nvm_area = nvm[nvm_index];
                    central_pool_t *cpool = &nvm_area->cpool;
                    span = acquire_span_from_centralpool1(cpool, index-1);
                    pthread_mutex_unlock(&nvm_area->lock);
                    break;
                }
                else
                {
                    if (nvm_index - 1 == -1)
                    {
                        nvm_index = nvm_list_area.nvm_index - 1;
                    }
                    else
                    {
                        nvm_index -= 1;
                    }
                }
            }
        }
        addr=span->free_start;
    }
    nvm_area_t *nvm_area = span->pool->nvm_area;
    central_pool_t *cpool = &nvm_area->cpool;
    pthread_mutex_lock(&nvm_area->lock);
    cpool->wear_count += 1;
    recalculate_priority(cpool);
    pthread_mutex_unlock(&nvm_area->lock);
    return addr;
}

void *hpnmalloc1(size_t size)
{
    check_init();
    void *addr = NULL;
    if (size == 0)
    {
        return addr;
    }
    if (size <= MAX_SMALL_BLOCK_SIZE)
    {
        addr = small_malloc(size);
    }
    else if (size <= MAX_LARGE_BLOCK_SIZE)
    {
        addr = large_malloc(size);
    }
    return addr;
}

static void small_free(span_t *span, void *ptr)
{

    local_cache_t *lc = local_cache;
    int size_of_small_block = (span->span_category + 1) * MIN_STORE_UNIT;
    span->sl_block_t.sm_b.free_block_num += 1;
    int free_block_num = span->sl_block_t.sm_b.free_block_num;
    if (free_block_num == span->sl_block_t.sm_b.total_block_num)
    {
        lc->foreground[span->span_category] = NULL;
        list_del(&span->span_free_list);
        nvm_area_t *nvm_area = span->pool->nvm_area;
        pthread_mutex_lock(&nvm_area->lock);
        span_back_to_centralpool(span);
        pthread_mutex_unlock(&nvm_area->lock);
    }
    else if (free_block_num == 1)
    {
        list_head *block = ptr;
        list_add_tail(block, &span->sl_block_t.sm_b.block_free_list);
        list_add_tail(&span->span_free_list, &lc->background[span->span_category]);
    }
    else
    {
        list_head *block = ptr;
        list_add_tail(block, &span->sl_block_t.sm_b.block_free_list);
    }
}

static void large_free(span_t *span)
{
    nvm_area_t *nvm_area = span->pool->nvm_area;
    pthread_mutex_lock(&nvm_area->lock);
    span_back_to_centralpool(span);
    pthread_mutex_unlock(&nvm_area->lock);
}

void *hpnfree1(void *ptr)
{

    if (ptr == NULL)
    {
        return;
    }

    span_t *span = span_extract_header(ptr);

    if (span->span_category != 256)
    {
        small_free(span, ptr);
    }
    else
    {
        large_free(span);
    }
}