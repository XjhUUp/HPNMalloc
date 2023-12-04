#ifndef HPNMALLOC_H_
#define HPNMALLOC_H_



#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h>
#include "list.h"
#include "dlist.h"
#include <stdbool.h>

#define THREAD_LOCAL __attribute__ ((tls_model ("initial-exec"))) __thread

//span相关
#define SPAN_SIZE                   (sizeof(span_t))                   
#define LARGE_OF_BITMAP             8
#define MASK                        0xFFFFFFFFFFFFFFFF

//全局模块
#define PAGE_SIZE                   4096
#define APPLY_VIRTUAL_MEMORY        1024*1024*1024
#define APPLY_SPAN_HEAD_MEMORY      APPLY_VIRTUAL_MEMORY/PAGE_SIZE*SPAN_SIZE  
#define PAGE_FROM_POOL_NUM          16
#define FIRST_LEVEL_SPAN_MAP_SIZE   512
#define APPLY_SPAN_MAP_MEMORY       APPLY_VIRTUAL_MEMORY/PAGE_SIZE*8         



//起始地址相关
#define RAW_POOL_START              (void*)(0x600000000000)
#define SPAN_HEAD_POOL_START        ((void*)(0x670000000000))

//内存块相关
#define MIN_STORE_UNIT              64
#define SMALL_BLOCK_NUM             64
#define LARGE_BLOCK_NUM             257
#define MAX_SMALL_BLOCK_SIZE        PAGE_SIZE
#define MAX_LARGE_BLOCK_SIZE        256*PAGE_SIZE

//磨损相关
#define FIXED_THRESHOLD            10 
#define BALANCE_FACTOR             1000 //平衡因子，10的3次方
#define WEAR_FACTOR                1000000000
typedef struct nvm_hardware_parameter_table nvm_hp_table_t;
typedef struct nvm_area nvm_area_t;
typedef struct nvm_list nvm_list_t;

typedef struct small_block_bitmap small_block_bitmap_t;
typedef struct span span_t;
typedef struct central_pool central_pool_t;
typedef struct local_cache local_cache_t;
typedef struct large_block_free_list large_block_free_list_t;
typedef struct span_head_pool span_head_pool_t;




typedef enum {
    UNINIT,
    INITED
} thread_state_t;

struct small_block_bitmap{
    uint64_t bitmap;
    uint16_t last_pos;
    uint16_t cur_pos;
    uint16_t total_block_num;    
    uint16_t free_block_num;    
};

struct span{
    uint8_t span_category;         //256表示大内存块，其余表示小内存块的索引
    list_head span_free_list;   
    union sl_block{
        small_block_bitmap_t sm_b; 
        uint16_t page_num;
    }sl_block_t;
    uint32_t wear_count;             
    central_pool_t *pool;
    bool flag;                      //true表示不在中心堆中，false表示在
    void *free_start;
};

struct large_block_free_list{
    list_head free_list;
    list_head *max_wear_page_index;
};

struct central_pool{
    pthread_mutex_t lock;
    void *pool_start;
    void *pool_end;
    void *free_start;
    void *free_end;
    large_block_free_list_t lb_free_list[LARGE_BLOCK_NUM];  
    uint64_t max_wc_count;
    nvm_area_t *nvm_area;
};

struct span_head_pool{
    void* next_request_addr;
};

struct local_cache{
    span_t *foreground[SMALL_BLOCK_NUM];    
    list_head background[SMALL_BLOCK_NUM];
    list_head n_avaliable_list[SMALL_BLOCK_NUM];
    list_head free_list;
    uint32_t local_cache_wc_threshold;
};




struct nvm_hardware_parameter_table{
    //uint16_t read_delay[RAWDELAYN]; //读延时，单位ns
    //uint16_t write_delay[RAWDELAYN]; //写延时，单位ns
    uint32_t hardware_mc;       //硬件容量大小，单位MB
    uint32_t remain_mc;         //剩余容量大小，单位MB
    uint64_t memory_wear_limit; //  内存磨损上限，单位次数
    uint64_t memory_wear_remain;//内存剩余可磨损次数
    float    unit_capacity_mp;  //单位容量内存价格
    float    wear_weight;
};

struct nvm_area{
    nvm_hp_table_t hp_table;
    central_pool_t cpool;
    float priority;
    list_head nvm_list_node;
};

struct nvm_list{
    list_head nvm_list_head;
};

void init_nvm_area_list();

bool add_nvm_area(nvm_area_t* n_area);

bool del_nvm_area(nvm_area_t* n_area);

nvm_area_t *make_init_nvm_area(uint32_t hardware_mc,uint32_t remain_mc,uint64_t memory_wear_limit,uint64_t memory_wear_remain,float unit_capacity_mp,float wear_weight);

void init_priority(nvm_area_t *n_area);

nvm_area_t* select_highest_priority_nvm_area();

//分配内存
void *hpnmalloc(size_t size);

//释放内存
void *hpnfree(void *ptr);

#endif
