#ifndef CPMA_H_
#define CPMA_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h>
#include "list.h"
#include "dlist.h"
#include <stdbool.h>

#define THREAD_LOCAL __attribute__((tls_model("initial-exec"))) __thread

// NVM相关
#define NVM_FILE_PATH0               "/mnt/pmemdir2/back0"
#define NVM_FILE_PATH1               "/mnt/pmemdir2/back1"
#define NVM_FILE_PATH2               "/mnt/pmemdir2/back2"
#define NVM_FILE_PATH3               "/mnt/pmemdir2/back3"

#define APPLY_PHY_MEMORY 10 * 1024 * 1024

// span相关
#define SPAN_SIZE (sizeof(span_t))


// 全局模块
#define PAGE_SIZE 4096
// 一次性申请的虚拟内存大小
#define APPLY_VIRTUAL_MEMORY APPLY_PHY_MEMORY
// 一次性申请span结构体虚拟内存的大小
#define APPLY_SPAN_HEAD_MEMORY APPLY_VIRTUAL_MEMORY / PAGE_SIZE *SPAN_SIZE
#define PAGE_FROM_POOL_NUM 16
#define FIRST_LEVEL_SPAN_MAP_SIZE 512
// 一次性申请span map的虚拟内存大小
#define APPLY_SPAN_MAP_MEMORY APPLY_VIRTUAL_MEMORY / PAGE_SIZE * 8

// 起始地址相关
#define RAW_POOL_START0 (void *)(0x600000000000) //NVM硬件1
#define RAW_POOL_START1 (void *)(0x610000000000) //NVM硬件2
#define RAW_POOL_START2 (void *)(0x620000000000) //NVM硬件3
#define RAW_POOL_START3 (void *)(0x630000000000) //NVM硬件4


#define SPAN_HEAD_POOL_START0 ((void *)(0x670000000000))
#define SPAN_HEAD_POOL_START1 ((void *)(0x671000000000))
#define SPAN_HEAD_POOL_START2 ((void *)(0x672000000000))
#define SPAN_HEAD_POOL_START3 ((void *)(0x673000000000))
// span_map部分
#define SPAN_MAP_POOL_START0 ((void *)(0x674000000000))
#define SPAN_MAP_POOL_START1 ((void *)(0x675000000000))
#define SPAN_MAP_POOL_START2 ((void *)(0x676000000000))
#define SPAN_MAP_POOL_START3 ((void *)(0x677000000000))

// 内存块相关
#define MIN_STORE_UNIT 64
#define SMALL_BLOCK_NUM 64
#define LARGE_BLOCK_NUM 257
#define MAX_SMALL_BLOCK_SIZE PAGE_SIZE
#define MAX_LARGE_BLOCK_SIZE 256 * PAGE_SIZE

// 磨损相关
#define FIXED_THRESHOLD 10
#define BALANCE_FACTOR 1000 // 平衡因子，10的3次方
//#define WEAR_FACTOR 1000000000
typedef struct nvm_hardware_parameter_table nvm_hp_table_t;
typedef struct nvm_area nvm_area_t;
typedef struct nvm_list nvm_list_t;

typedef struct small_block_bitmap small_block_bitmap_t;
typedef struct span span_t;
typedef struct central_pool central_pool_t;
typedef struct local_cache local_cache_t;
typedef struct large_block_free_list large_block_free_list_t;
typedef struct span_head_pool span_head_pool_t;
typedef struct span_map_pool  span_map_pool_t;

typedef enum
{
    UNINIT,
    INITED
} thread_state_t;

struct small_block_bitmap
{
    uint16_t total_block_num;
    uint16_t free_block_num;
    void *avaliable_free_start;
    //void *end_free_start;
    list_head block_free_list;
};

struct span
{
    list_head span_free_list;
    union sl_block
    {
        small_block_bitmap_t sm_b;
        uint16_t page_num;
    } sl_block_t;
    //uint32_t wear_count;
    central_pool_t *pool;
    //bool flag; // true表示不在中心堆中，false表示在
    void *free_start;
    uint16_t span_category; // 256表示大内存块，其余表示小内存块的索引
};


struct central_pool
{
    pthread_mutex_t lock;
    void *pool_start;
    void *pool_end;
    void *free_start;
    void *free_end;
    list_head lb_free_list[LARGE_BLOCK_NUM];
    //uint64_t max_wc_count;
    uint64_t wear_count;
    nvm_area_t *nvm_area;
    // NVM相关
    void *next_nvm_start;
    int fd;

    void *span_head_pool_start;
    void *next_request_addr;
    void *next_span_map_addr;
    void *span_map[FIRST_LEVEL_SPAN_MAP_SIZE];

    int wear_limit;
};


struct local_cache
{
    span_t *foreground[SMALL_BLOCK_NUM];
    list_head background[SMALL_BLOCK_NUM];
    list_head free_list;
};

struct nvm_hardware_parameter_table
{
    double hardware_mc;        // 硬件容量大小，单位MB
    double used_mc;            // 已使用容量大小，单位MB
    uint64_t memory_wear_limit;  //  内存磨损上限，单位次数
    uint64_t memory_wear_remain; // 内存剩余可磨损次数
    double unit_capacity_mp;      // 单位容量内存价格
    double wear_weight;
};

struct nvm_area
{
    nvm_hp_table_t hp_table;
    central_pool_t cpool;
    double priority;
    pthread_mutex_t lock;
};

struct nvm_list
{
    nvm_area_t *nvm[10];
    int nvm_index;

};

void init_nvm_area_list();

bool add_nvm_area(nvm_area_t *n_area);

bool del_nvm_area(nvm_area_t *n_area);

nvm_area_t *make_init_nvm_area(double hardware_mc, double remain_mc, uint64_t memory_wear_limit, uint64_t memory_wear_remain, double unit_capacity_mp, double wear_weight);

void init_priority(nvm_area_t *n_area);

// void rearrange_nvm();


// 分配内存
void *cpmalloc(size_t size);

// 释放内存
void *cpfree(void *ptr);

#endif
