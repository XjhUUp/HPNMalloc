#ifndef HPNMALLOC_H_
#define HPNMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdint.h>
#include "list.h"
#include "dlist.h"
#include <stdbool.h>

#define THREAD_LOCAL __attribute__ ((tls_model ("initial-exec"))) __thread


#define PAGE_SIZE 4096
#define SMALL_BLOCK_NUM 64
#define LARGE_BLOCK_NUM 256

#define MAX_SMALL_BLOCK_SIZE    PAGE_SIZE
#define MAX_LARGE_BLOCK_SIZE    256*PAGE_SIZE

#define SPAN_SIZE                   (sizeof(span_t))
#define RAW_POOL_START              ((void*)((0x600000000000/PAGE_SIZE+1)*PAGE_SIZE))
#define INITIAL_VIRTUAL_MEMORY      1024*1024 //1MB
#define INITAL_PHY_MEMORY           1024*PAGE_SIZE

#define MIN_STORE_UNIT 64

#define FIXED_THRESHOLD            10 

#define LARGE_OF_BITMAP            8

#define Central_TO_Local_Page_Num  16

//#define BITMAP_ARRAY_NUM    64

#define MASK 0xFFFFFFFFFFFFFFFF

typedef struct small_block_bitmap small_block_bitmap_t;
typedef struct span span_t;
typedef struct central_pool central_pool_t;
typedef struct local_cache local_cache_t;
typedef struct large_block_free_list large_block_free_list_t;
typedef struct small_block_return_signal small_block_return_signal_t;

typedef enum {
    UNINIT,
    INITED
} thread_state_t;

typedef enum {
    FORG,
    BACK,
    FULL,
    NAVA,   /* NOT AVAILABLE, ready to return to global when all clean */
} span_state_t;


struct small_block_bitmap{
    uint64_t bitmap;
    uint16_t last_pos;
    uint16_t cur_pos;
    uint16_t total_block_num;    //总的数据块数量
    uint16_t free_block_num;     //总的空闲块数量
    span_state_t state;
    local_cache_t *local_cache;

};



struct span{
    //void *span_free_start;
    uint8_t block_category;        // 256表示内存块 其余表示小内存块的索引
    list_head span_free_list;   //span的链表节点
    union sl_block{
        small_block_bitmap_t sm_b;
        uint16_t page_num;
    }sl_block_t;
    int wear_count;             //磨损计数值
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
    int page_num;

};



struct local_cache{
    span_t *foreground[SMALL_BLOCK_NUM];
    list_head background[SMALL_BLOCK_NUM];
    list_head n_avaliable_list[SMALL_BLOCK_NUM];
    list_head free_list;
    int local_cache_wc_threshold;
};

struct small_block_return_signal{
    list_head *list;
    bool flag;  //0表示连续   1表示不连续
};

//分配内存
void *hpnmalloc(size_t size);

//释放内存
void *hpnfree(void *ptr);

#ifdef __cplusplus
}
#endif
#endif
