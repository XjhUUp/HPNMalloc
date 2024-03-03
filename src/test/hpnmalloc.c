#include "pthread.h"
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hpnmalloc.h"

pthread_once_t init_once = PTHREAD_ONCE_INIT;
thread_state_t global_state = UNINIT;
pthread_key_t destructor;
THREAD_LOCAL thread_state_t thread_state = UNINIT;
THREAD_LOCAL local_cache_t *local_cache = NULL;

nvm_area_t *max_priority_nvm_area;
central_pool_t *central_pool;
span_head_pool_t head_pool;
//span_map部分
span_map_pool_t span_map_pool;
extern nvm_list_t nvm_list_area;

void *span_map[FIRST_LEVEL_SPAN_MAP_SIZE];


int bitmap_auxiliary_array[32] = {
    1, 2, 29, 3, 30, 15, 25, 4, 31, 23, 21, 16, 26, 18, 5, 9,
    32, 28, 14, 24, 22, 20, 17, 8, 27, 13, 19, 7, 12, 6, 11, 10};

#define LOW1BIT(X) (bitmap_auxiliary_array[((unsigned int)(X * 0x077CB531U)) >> 27])

static void *page_alloc(void *pos, size_t size)
{
    return mmap(pos, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
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

static void central_pool_init(central_pool_t *central_pool, nvm_area_t *nvm_area)
{
    if (pthread_mutex_init(&central_pool->lock, NULL) < 0)
    {
        fprintf(stderr, "fatal error: pthread_mutex_init failed\n");
        exit(-1);
    }

    // void *ret = page_alloc(RAW_POOL_START, APPLY_VIRTUAL_MEMORY);
    // if ((ret < (int)0))
    // {
    //     fprintf(stderr, "fatal error: page_alloc ailed\n");
    //     exit(-1);
    // }
    // central_pool->pool_start = (void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);
    // central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    // central_pool->free_start = central_pool->pool_start;
    // central_pool->free_end = central_pool->pool_end;
    // for (int i = 0; i < LARGE_BLOCK_NUM; i++)
    // {
    //     INIT_LIST_HEAD(&central_pool->lb_free_list[i].free_list);
    //     central_pool->lb_free_list[i].max_wear_page_index = NULL;
    // }
    // central_pool->nvm_area = nvm_area;
    // central_pool->max_wc_count = 0;

    void *ret=RAW_POOL_START;
    ret=(void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);

    /*
        NVM部分
        1. 打开或创建文件
        2. 扩张文件
    */
    int fd = open_empty_or_create_file(NVM_FILE_PATH);
    central_pool->next_nvm_start=0;
    central_pool->fd=fd;
    if (posix_fallocate(fd, 0, APPLY_PHY_MEMORY) != 0)
    {
        fprintf(stderr, "nvm_alloc failed\n");
    }
    if (mmap(ret, APPLY_PHY_MEMORY, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED, fd, central_pool->next_nvm_start) == MAP_FAILED){
         fprintf(stderr, "mmap failed\n");
    }
    central_pool->next_nvm_start+=APPLY_PHY_MEMORY;

    central_pool->pool_start =ret;
    central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    central_pool->free_start = central_pool->pool_start;
    central_pool->free_end = central_pool->pool_end;
    for (int i = 0; i < LARGE_BLOCK_NUM; i++)
    {
        INIT_LIST_HEAD(&central_pool->lb_free_list[i].free_list);
        central_pool->lb_free_list[i].max_wear_page_index = NULL;
    }
    central_pool->nvm_area = nvm_area;
    central_pool->max_wc_count = 0;
}

static void init_head_pool()
{
    head_pool.next_request_addr = SPAN_HEAD_POOL_START;
    void *span_head_addr = page_alloc(SPAN_HEAD_POOL_START, APPLY_SPAN_HEAD_MEMORY);
    if(span_head_addr<0){
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    head_pool.next_request_addr = span_head_addr + APPLY_SPAN_HEAD_MEMORY;
}

static void init_nvm()
{
    nvm_area_t *n_area0 = make_init_nvm_area(128, 128, 100000000000 / WEAR_FACTOR, 100000000000 / WEAR_FACTOR, 4.51, 1);
    central_pool_init(&n_area0->cpool, n_area0);
    // nvm_area_t *n_area1 = make_init_nvm_area(128, 128, 100000000000 / WEAR_FACTOR, 100000000000 / WEAR_FACTOR, 4.51, 1);
    // central_pool_init(&n_area1->cpool,n_area1);
    // nvm_area_t *n_area2 = make_init_nvm_area(128, 128, 100000000000 / WEAR_FACTOR, 100000000000 / WEAR_FACTOR, 4.51, 1);
    // central_pool_init(&n_area2->cpool,n_area2);
    // nvm_area_t *n_area3 = make_init_nvm_area(128, 128, 100000000000 / WEAR_FACTOR, 100000000000 / WEAR_FACTOR, 4.51, 1);
    // central_pool_init(&n_area3->cpool,n_area3);
}

static void init_span_map()
{
    span_map_pool.next_span_map_addr=SPAN_MAP_POOL_START;
    void *addr = page_alloc(SPAN_MAP_POOL_START,APPLY_SPAN_MAP_MEMORY);
    if(addr<0){
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    span_map[0] = addr;
    span_map_pool.next_span_map_addr=addr+APPLY_SPAN_MAP_MEMORY;
}

static void global_init()
{
    init_head_pool();
    init_nvm_area_list();
    init_nvm();
    init_span_map();
    max_priority_nvm_area = select_highest_priority_nvm_area();
    central_pool = &max_priority_nvm_area->cpool;
    global_state = INITED;
}

static local_cache_t *init_local_cache()
{
    local_cache_t *local_cache = (local_cache_t *)malloc(sizeof(local_cache_t));
    local_cache->local_cache_wc_threshold = 0;
    for (int i = 0; i < SMALL_BLOCK_NUM; i++)
    {
        local_cache->foreground[i] = NULL;
        INIT_LIST_HEAD(&(local_cache->background[i]));
        INIT_LIST_HEAD(&(local_cache->n_avaliable_list[i]));
    }
    INIT_LIST_HEAD(&local_cache->free_list);
    return local_cache;
}

static void recalculate_priority(central_pool_t *pool)
{
    nvm_area_t *narea = pool->nvm_area;
    float w = (float)1 - (float)narea->cpool.max_wc_count / (float)narea->hp_table.memory_wear_limit;
    if (narea->hp_table.wear_weight - w >= 0.1)
    {
        narea->hp_table.wear_weight = narea->hp_table.wear_weight - 0.1;
        narea->hp_table.memory_wear_remain = narea->hp_table.memory_wear_limit - narea->cpool.max_wc_count;
        init_priority(narea);
        nvm_area_t *nvm_area=select_highest_priority_nvm_area();
        if(nvm_area!=max_priority_nvm_area){
            central_pool_t *pool_temp=central_pool;
            max_priority_nvm_area=nvm_area;
            central_pool = &max_priority_nvm_area->cpool;
        }
    }
}

static span_t *span_extract_header(void *ptr)
{
    // 计算是哪一段内存
    int memory_index = (ptr - RAW_POOL_START) / (APPLY_VIRTUAL_MEMORY);
    // 计算是该段内存的哪一个索引
    int span_index = (ptr - RAW_POOL_START) / PAGE_SIZE;
    span_index=span_index%(APPLY_PHY_MEMORY/PAGE_SIZE);
    // 获取对应的地址段的首地址
    void *addr = span_map[memory_index];
    // 获取存放span地址的地址
    addr = addr + span_index * 8;
    // 取出地址值
    span_t *span = *(uint64_t *)addr;
    return span;
}

/*
    函数作用：查找前一个span是否空闲，若空闲则合并
*/
static span_t *merge_span(span_t *span)
{
    span_t *pre_span = NULL;
    void *ptr = span->free_start;
    ptr -= 1;
    if (ptr < RAW_POOL_START)
    {
        return span;
    }
    pre_span = span_extract_header(ptr);
    if (pre_span->flag == false)
    {
        if (span->wear_count > pre_span->wear_count)
        {
            pre_span->wear_count = span->wear_count;
        }
        pre_span->sl_block_t.page_num += span->sl_block_t.page_num;
        list_del(&pre_span->span_free_list);
        return pre_span;
    }
    return span;
}

static void span_back_to_centralpool(span_t *span)
{
    central_pool_t *central_pool = span->pool;
    int index;
    span->flag = false;
    if (span->span_category != 256)
    {
        span->sl_block_t.page_num = 1;
    }
    span = merge_span(span);
    int page_num = span->sl_block_t.page_num;
    if (page_num <= 256)
    {
        index = page_num - 1;
    }
    else
    {
        index = 256;
    }
    if (span->wear_count > central_pool->max_wc_count)
    {
        central_pool->max_wc_count = span->wear_count;
        list_add_tail(&span->span_free_list, &central_pool->lb_free_list[index].free_list);
        central_pool->lb_free_list[index].max_wear_page_index = &span->span_free_list;
        for (int i = 0; i < LARGE_BLOCK_NUM; i++)
        {
            if (i == index)
            {
                continue;
            }
            central_pool->lb_free_list[index].max_wear_page_index = NULL;
        }
        recalculate_priority(central_pool);
    }
    else if (span->wear_count == central_pool->max_wc_count)
    {
        list_add_tail(&span->span_free_list, &central_pool->lb_free_list[index].free_list);
        if (central_pool->lb_free_list[index].max_wear_page_index == NULL)
        {
            central_pool->lb_free_list[index].max_wear_page_index = &span->span_free_list;
        }
    }
    else
    {
        if (central_pool->lb_free_list[index].max_wear_page_index == NULL)
        {
            list_add_tail(&span->span_free_list, &central_pool->lb_free_list[index].free_list);
            central_pool->lb_free_list[index].max_wear_page_index = &span->span_free_list;
        }
        else
        {
            list_add(&span->span_free_list, &central_pool->lb_free_list[index].max_wear_page_index->prev);
        }
    }
}

static void thread_exit()
{
    span_t *span = NULL;
    //pthread_mutex_lock(&central_pool->lock);
    for (int i = 0; i < SMALL_BLOCK_NUM; i++)
    {
        if (local_cache->foreground[i] != NULL)
        {
            span = local_cache->foreground[i];
            list_del(&span->span_free_list);
            pthread_mutex_lock(&span->pool->lock);
            span_back_to_centralpool(span);
            pthread_mutex_unlock(&span->pool->lock);
        }
    }
    for (int i = 0; i < SMALL_BLOCK_NUM; i++)
    {
        while (!list_empty(&local_cache->background[i]))
        {
            span = local_cache->background[i].next;
            list_del(&span->span_free_list);
            pthread_mutex_lock(&span->pool->lock);
            span_back_to_centralpool(span);
            pthread_mutex_unlock(&span->pool->lock);
        }
    }
    for (int i = 0; i < SMALL_BLOCK_NUM; i++)
    {
        while (!list_empty(&local_cache->n_avaliable_list[i]))
        {
            span = local_cache->n_avaliable_list[i].next;
            list_del(&span->span_free_list);
            pthread_mutex_lock(&span->pool->lock);
            span_back_to_centralpool(span);
            pthread_mutex_unlock(&span->pool->lock);
        }
    }
    while (!list_empty(&local_cache->free_list))
    {
        span = local_cache->free_list.next;
        list_del(&span->span_free_list);
        pthread_mutex_lock(&span->pool->lock);
        span_back_to_centralpool(span);
        pthread_mutex_unlock(&span->pool->lock);
    }

    //pthread_mutex_unlock(&central_pool->lock);
}

static void thread_init()
{
    pthread_setspecific(destructor, thread_exit);
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

static void central_pool_grow()
{
    // void *addr = page_alloc(central_pool->pool_end, APPLY_VIRTUAL_MEMORY);
    // if ((addr < 0) || (span_head_addr < 0))
    // {
    //     fprintf(stderr, "page_alloc failed\n");
    //     exit(-1);
    // }
    int fd=central_pool->fd;
    void *ret=central_pool->pool_end;
    int pos=central_pool->next_nvm_start;
    if (posix_fallocate(fd, pos, APPLY_PHY_MEMORY) != 0)
    {
        fprintf(stderr, "nvm_alloc failed\n");
    }
    if (mmap(ret, APPLY_PHY_MEMORY, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED, fd, pos) == MAP_FAILED){
         fprintf(stderr, "mmap failed\n");
    }

    void *span_head_addr = page_alloc(head_pool.next_request_addr, APPLY_SPAN_HEAD_MEMORY);
    if(span_head_addr<0){
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    head_pool.next_request_addr = span_head_addr + APPLY_SPAN_HEAD_MEMORY;
    central_pool->free_start = (void *)(((uint64_t)ret + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);
    central_pool->pool_end = ret + APPLY_VIRTUAL_MEMORY;
    central_pool->free_end = central_pool->pool_end;
    // 扩展span_map
    int memory_index = (ret - RAW_POOL_START) / (APPLY_VIRTUAL_MEMORY);
    void *span_map_addr =page_alloc(span_map_pool.next_span_map_addr,APPLY_SPAN_MAP_MEMORY);
    if(span_map_addr<0){
        fprintf(stderr, "fatal error: page_alloc ailed\n");
    }
    span_map_pool.next_span_map_addr=span_map_addr+APPLY_SPAN_MAP_MEMORY;
    // 计算是哪一段内存
    span_map[memory_index] = span_map_addr;
}

/*
    函数作用：初始化或者需要修改page对应的span的时候调用
*/
static void reset_span_map(span_t *span)
{
    void *span_addr = span;
    // 计算是哪一段内存
    size_t memory_index = (span->free_start - RAW_POOL_START) / (APPLY_VIRTUAL_MEMORY);

    // 计算是该段内存的哪一个索引
    int span_index = (span->free_start - RAW_POOL_START) / PAGE_SIZE;
    span_index=span_index%(APPLY_PHY_MEMORY/PAGE_SIZE);
    // 获取对应内存地址段的首地址
    void *addr = span_map[memory_index];
    // 获取存放span地址的指针
    addr = addr + span_index * 8;
    // 获取要存放的span的64位地址值
    uint64_t s_addr = (uint64_t)span;
    // 将64位地址值保存
    *(uint64_t *)addr = s_addr;
}

/*
    1. span_category表示是小内存的span还是大内存块的span
    2. index表示span的索引
*/
static void span_init(span_t *span, int index, uint8_t span_category, void *free_start, bool flag, bool span_map_flag)
{
    if (span_category != 256)
    {
        span->span_category = index;
        span->sl_block_t.sm_b.total_block_num = PAGE_SIZE / ((index + 1) * 64);
        span->sl_block_t.sm_b.cur_pos = 0;
        span->sl_block_t.sm_b.last_pos = span->sl_block_t.sm_b.total_block_num;
        span->sl_block_t.sm_b.free_block_num = span->sl_block_t.sm_b.total_block_num;
        span->sl_block_t.sm_b.bitmap = (MASK >> (64 - span->sl_block_t.sm_b.total_block_num));
    }
    else
    {
        span->span_category = 256;
        span->sl_block_t.page_num = index + 1;
    }
    span->pool = &max_priority_nvm_area->cpool;
    span->free_start = free_start;
    span->flag = flag;
    INIT_LIST_HEAD(&span->span_free_list);
    if (span_map_flag == true)
    {
        reset_span_map(span);
    }
}

static span_t *span_split(list_head *list_node, int cur_index, int target_index)
{
    span_t *span = list_entry(list_node, span_t, span_free_list);
    if (cur_index == 256)
    {
        cur_index = span->sl_block_t.page_num - 1;
    }
    span_t *span_s = (void *)span + target_index * SPAN_SIZE;
    int list_index_s = cur_index - target_index - 1;
    span_init(span_s, list_index_s, 256, span->free_start + target_index * PAGE_SIZE, false, true);
    if (span_s->wear_count == central_pool->max_wc_count)
    {
        if (list_index_s > 255)
        {
            list_add_tail(&span->span_free_list, &central_pool->lb_free_list[LARGE_BLOCK_NUM - 1].free_list);
        }
        else
        {
            list_add_tail(&span->span_free_list, &central_pool->lb_free_list[list_index_s].free_list);
        }
    }
    else
    {
        if (list_index_s > 255)
        {
            list_add(&span->span_free_list, &central_pool->lb_free_list[LARGE_BLOCK_NUM - 1].free_list);
        }
        else
        {
            list_add(&span->span_free_list, &central_pool->lb_free_list[list_index_s].free_list);
        }
    }
    return span;
}

/*
    1. 从localcache获取span并初始化
*/
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

/*
    1. 提供span,不负责初始化
    2. 参数index表示从中心堆的索引为index的链表中开始找
*/
static span_t *acquire_span(int index)
{
    span_t *span = NULL;
    while (1)
    {
        if (central_pool->free_start+(index+1)*PAGE_SIZE < central_pool->free_end)
        {
            goto finsh;
            break;
        }
        else
        {
            if (!list_empty(&central_pool->lb_free_list[index].free_list))
            {
                if (central_pool->lb_free_list[index].max_wear_page_index != NULL && central_pool->lb_free_list[index].max_wear_page_index == central_pool->lb_free_list[index].free_list.next)
                {
                    if (central_pool->lb_free_list[index].free_list.next->next == &central_pool->lb_free_list[index].free_list)
                    {
                        central_pool->lb_free_list[index].max_wear_page_index = NULL;
                    }
                    else
                    {
                        central_pool->lb_free_list[index].max_wear_page_index = central_pool->lb_free_list[index].free_list.next->next;
                    }
                }
                span = list_entry(central_pool->lb_free_list[index].free_list.next, span_t, span_free_list);
                list_del(&span->span_free_list);
                return span;
            }
            else
            {
                for (int i = index + 1; i < LARGE_BLOCK_NUM; i++)
                {
                    if (!list_empty(&central_pool->lb_free_list[i].free_list))
                    {
                        if (central_pool->lb_free_list[i].max_wear_page_index != NULL && central_pool->lb_free_list[i].max_wear_page_index == central_pool->lb_free_list[i].free_list.next)
                        {
                            if (central_pool->lb_free_list[i].free_list.next->next == &central_pool->lb_free_list[i].free_list)
                            {
                                central_pool->lb_free_list[i].max_wear_page_index = NULL;
                            }
                            else
                            {
                                central_pool->lb_free_list[i].max_wear_page_index = central_pool->lb_free_list[i].free_list.next->next;
                            }
                        }
                        if (i == LARGE_BLOCK_NUM - 1)
                        {
                            span = span_split(central_pool->lb_free_list[i].free_list.next, 256, index);
                        }
                        else
                        {
                            span = span_split(central_pool->lb_free_list[i].free_list.next, i, index);
                        }
                        list_del(&span->span_free_list);
                        return span;
                    }
                }
            }
        }
        central_pool_grow();
        int span_index;
    finsh:
        // 计算是第几个span
        span_index = (central_pool->free_start - RAW_POOL_START) / PAGE_SIZE;
        span = SPAN_HEAD_POOL_START + (span_index * SPAN_SIZE);
        span->wear_count = 0;
        span->free_start = central_pool->free_start;
        central_pool->free_start += PAGE_SIZE * (index + 1);
        break;
    }
    return span;
}

/*
    1. 从centralpool获取span并初始化
    2. 从centralpool申请一些span到localcache作为缓存
    3. 更新localcache的阈值
*/
span_t *acquire_span_from_centralpool(int index)
{
    int span_max_wear_count;
    span_t *span = NULL;
    pthread_mutex_lock(&central_pool->lock);
    span = acquire_span(0);
    span_max_wear_count=span->wear_count;
    span_init(span, index, index, span->free_start, true, true);
    span_t *span_s = NULL;
    for (int i = 0; i < PAGE_FROM_POOL_NUM; i++)
    {
        span_s = acquire_span(0);
        if(span_s->wear_count>span_max_wear_count){
            span_max_wear_count=span_s->wear_count;
        }
        list_add_tail(&span_s->span_free_list, &local_cache->free_list);
    }
    pthread_mutex_unlock(&central_pool->lock);
    reset_localcache_wc_threshold(span_max_wear_count);
    return span;
}

/*
    函数作用：重置线程缓存的最大磨损值
    1. 如果中心堆的最大磨损度和分配的span的磨损度的差值小于一定值（FIXED_THRESHOLD），那么线程缓存的最大磨损度就是span的磨损度加上这么一个值，目的就是为了让线程缓存的磨损度大于中心堆的最大磨损度
    2. 如果中心堆的最大磨损度和分配的span的磨损度的差值大于等于一定值（FIXED_THRESHOLD），那么线程缓存的最大磨损度就是中心堆的最大磨损度
    3. 综上所属就是使的线程缓存的最大磨损度和中心堆的最大磨损度的值控制在（FIXED_THRESHOLD）内（线程缓存的磨损度大于中心堆的磨损度）
*/

void reset_localcache_wc_threshold(uint32_t span_wear_count)
{
    int wc = 0;
    if (central_pool->max_wc_count - span_wear_count < FIXED_THRESHOLD)
    {
        wc = span_wear_count + FIXED_THRESHOLD;
    }
    else
    {
        wc = central_pool->max_wc_count;
    }
    if (wc != local_cache->local_cache_wc_threshold)
    {
        local_cache->local_cache_wc_threshold = wc;
        for (int i = 0; i < SMALL_BLOCK_NUM; i++)
        {
            while (!list_empty(&local_cache->n_avaliable_list[i]))
            {
                list_head *node = local_cache->n_avaliable_list[i].next;
                list_del(node);
                list_add_tail(node, &local_cache->background[i]);
            }
        }
    }
}

/*
    函数作用：采用顺序分配法分配span中的内存块
*/
void *span_clock_allocate(span_t *span)
{
    uint64_t bitmap = span->sl_block_t.sm_b.bitmap;
    int16_t cur_pos = span->sl_block_t.sm_b.cur_pos;
    uint16_t last_pos = span->sl_block_t.sm_b.last_pos;
    uint64_t flag;
    uint64_t flag_a = 0xFFFFFFFFFFFFFFFF;
    flag = flag_a << cur_pos;
    // 需要从头开始扫描
    if ((cur_pos == last_pos) || (bitmap & flag) == 0)
    {
        span->wear_count += 1;
        if (span->wear_count >= local_cache->local_cache_wc_threshold)
        {
            if (span->sl_block_t.sm_b.free_block_num == span->sl_block_t.sm_b.total_block_num)
            {
                list_del(&span->span_free_list);
                pthread_mutex_lock(&span->pool->lock);
                span_back_to_centralpool(span);
                pthread_mutex_unlock(&span->pool->lock);
            }
            else
            {
                list_del(&span->span_free_list);
                list_add_tail(&span->span_free_list, &local_cache->n_avaliable_list[span->span_category]);
            }
            local_cache->foreground[span->span_category] = NULL;
            return NULL;
        }
        cur_pos = 0;
    }
    // 先把cur_pos之前的位置0，然后找出最低位的1，并把其他位置0，然乎查表
    flag = flag_a << cur_pos;
    bitmap = bitmap & flag;
    bitmap = bitmap & (~bitmap + 1);
    if (bitmap & 0x00000000FFFFFFFF)
    {
        cur_pos = LOW1BIT((bitmap & 0x00000000FFFFFFFF));
    }
    else
    {
        bitmap >>= 32;
        cur_pos = LOW1BIT((bitmap & 0x00000000FFFFFFFF)) + 32;
    }
    span->sl_block_t.sm_b.cur_pos = cur_pos;
    span->sl_block_t.sm_b.bitmap &= ~((uint64_t)1 << (cur_pos - 1));
    // return (void *)span+SPAN_SIZE+(cur_pos-1)*(span->block_category+1)*MIN_STORE_UNIT;
    return (void *)span->free_start + (cur_pos - 1) * (span->span_category + 1) * MIN_STORE_UNIT;
}

/*
    函数作用：申请小内存块
    1. 首先从线程缓存中申请
    2. 从中心堆进行申请
    3. 采用时钟顺序法分配内存块
    4. 重置span的状态
*/
static void *small_malloc(size_t size)
{
    void *addr = NULL;
    local_cache_t *lc = local_cache;
    span_t *span = NULL;
    int index = (size - 1) / MIN_STORE_UNIT;
    while (addr == NULL)
    {
        span = acquire_span_from_localcache(index);
        if (span == NULL)
        {
            span = acquire_span_from_centralpool(index);
        }
        lc->foreground[index] = span;
        addr = span_clock_allocate(span);
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
    return addr;
}

void *hpnmalloc(size_t size)
{
    check_init();
    void *addr = NULL;
    if(size==0){
        return addr;
    }
    if (size <= MAX_SMALL_BLOCK_SIZE)
    {
        addr = small_malloc(size);
    }
    else if (size <= MAX_LARGE_BLOCK_SIZE)
    {
        // addr = large_malloc(size);
    }
    return addr;
}

/*
    函数作用：回收小内存块
*/
static void small_free(span_t *span, void *ptr)
{
    local_cache_t *lc = local_cache;
    int size_of_small_block = (span->span_category + 1) * MIN_STORE_UNIT;
    span->sl_block_t.sm_b.free_block_num += 1;
    int free_block_num = span->sl_block_t.sm_b.free_block_num;
    if (free_block_num == span->sl_block_t.sm_b.total_block_num)
    {
        if (span->wear_count >= lc->local_cache_wc_threshold)
        {
            list_del(&span->span_free_list);
            pthread_mutex_lock(&span->pool->lock);
            span_back_to_centralpool(span);
            pthread_mutex_unlock(&span->pool->lock);
            if(lc->foreground[span->span_category]==span){
                lc->foreground[span->span_category]=NULL;
            }
        }
        else
        {
            list_del(&span->span_free_list);
            list_add_tail(&span->span_free_list, &lc->free_list);
            if(lc->foreground[span->span_category]==span){
                lc->foreground[span->span_category]=NULL;
            }
        }
    }
    else if (free_block_num == 1)
    {
        int index = ((uint64_t)ptr - (uint64_t)span->free_start) / size_of_small_block + 1;
        span->sl_block_t.sm_b.bitmap |= (uint64_t)1 << (index - 1);
        if (span->wear_count < lc->local_cache_wc_threshold)
        {
            list_add_tail(&span->span_free_list, &lc->background[span->span_category]);
        }
    }
    else
    {
        int index = ((uint64_t)ptr - (uint64_t)span->free_start) / size_of_small_block + 1;
        span->sl_block_t.sm_b.bitmap |= (uint64_t)1 << (index - 1);
    }
}

void *hpnfree(void *ptr)
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
        // large_free(span);
    }
}