#include "pthread.h"
#include <math.h>
#include "hpnmalloc.h"



pthread_once_t init_once = PTHREAD_ONCE_INIT;
thread_state_t global_state = UNINIT;
pthread_key_t destructor;

central_pool_t central_pool;

int max_wc_couunt;

THREAD_LOCAL thread_state_t thread_state=UNINIT;
THREAD_LOCAL local_cache_t *local_cache=NULL;

int bitmap_auxiliary_array[32]={
	1, 2, 29, 3, 30, 15, 25, 4, 31, 23, 21, 16, 26, 18, 5, 9,
		32, 28, 14, 24, 22, 20, 17, 8, 27, 13, 19, 7, 12, 6, 11, 10
};

#define LOW1BIT(X) (bitmap_auxiliary_array[((unsigned int)(X * 0x077CB531U)) >> 27])



static void *page_alloc(void *pos, size_t size) {
    return mmap(pos, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}



static void central_pool_init(){
    if(pthread_mutex_init(&central_pool.lock,NULL)<0){
        fprintf(stderr, "fatal error: pthread_mutex_init failed\n");
        exit(-1);
    }
    void *ret=page_alloc(RAW_POOL_START,INITIAL_VIRTUAL_MEMORY);
    if (ret < (int)0) {
        fprintf(stderr, "fatal error: page_alloc ailed\n");
        exit(-1);
    }
    central_pool.pool_start=(void *)(((uint64_t)ret + PAGE_SIZE - 1)/PAGE_SIZE * PAGE_SIZE);
    central_pool.pool_end=ret+INITIAL_VIRTUAL_MEMORY;
    central_pool.free_start=central_pool.pool_start;
    //central_pool.free_end=central_pool.free_start;
    central_pool.free_end=central_pool.pool_end;
    central_pool.page_num=0;
    for(int i=0;i<LARGE_BLOCK_NUM;i++){
        INIT_LIST_HEAD(&central_pool.lb_free_list[i].free_list);
        central_pool.lb_free_list[i].max_wear_page_index=NULL;
    }  
}


static void global_init(){
    //pthread_key_create(&destructor, thread_exit);
    max_wc_couunt=0;
    central_pool_init();
    global_state=INITED;

}

static local_cache_t *acquire_local_cache(){
    local_cache_t *local_cache=(local_cache_t *)malloc(sizeof(local_cache_t));
    local_cache->local_cache_wc_threshold=0;
    for(int i=0;i<SMALL_BLOCK_NUM;i++){
        local_cache->foreground[i]=NULL;
        INIT_LIST_HEAD(&(local_cache->background[i]));
        INIT_LIST_HEAD(&(local_cache->n_avaliable_list[i]));
    }
    INIT_LIST_HEAD(&local_cache->free_list);
    return local_cache;    
}


static void thread_init(){
    pthread_setspecific(destructor, (void *)1);
    local_cache=acquire_local_cache();
    thread_state=INITED;
}

static void check_init(){
    if(thread_state!=INITED){
        if(global_state!=INITED){
            pthread_once(&init_once,global_init);
        }
        thread_init();
    }
}

static void central_pool_grow(){
    //首先判断虚拟内存该不该扩展
    printf("---1\n");
    if(central_pool.free_start+INITAL_PHY_MEMORY>central_pool.pool_end){
        void *addr=page_alloc(central_pool.pool_end,INITIAL_VIRTUAL_MEMORY);
        if (addr < 0) {
            fprintf(stderr, "page_alloc failed\n");
            exit(-1);
        }
        central_pool.free_start=(void *)(((uint64_t)addr + PAGE_SIZE - 1)/PAGE_SIZE * PAGE_SIZE);
        central_pool.pool_end=central_pool.pool_end+INITIAL_VIRTUAL_MEMORY;
        central_pool.free_end=central_pool.pool_end;
    }
    //扩展物理内存，首先判断文件是否存在，存在则扩展文件并映射，不存在则创建文件
}

static void span_init(span_t *span, local_cache_t *local_cache,int index,uint8_t block_category){
    if(block_category!=256){
        span->sl_block_t.sm_b.local_cache=local_cache;
        span->block_category=index;
        span->sl_block_t.sm_b.total_block_num=(PAGE_SIZE-SPAN_SIZE)/((index+1)*64);
        span->sl_block_t.sm_b.state=FORG;
        span->sl_block_t.sm_b.cur_pos=0;
        span->sl_block_t.sm_b.last_pos=span->sl_block_t.sm_b.total_block_num;
        span->sl_block_t.sm_b.free_block_num=span->sl_block_t.sm_b.total_block_num;
        span->sl_block_t.sm_b.bitmap=(MASK>>(64-span->sl_block_t.sm_b.total_block_num));
    }else{
        span->block_category=256;
        span->sl_block_t.page_num=index+1;
    }
}


static span_t* span_split(list_head *list_node,int cur_index,int target_index){
    span_t *span=list_entry(list_node,span_t,span_free_list);
    span_t *span_s=(void *)span+target_index*PAGE_SIZE;
    int list_index_s=cur_index-target_index-1;
    span_init(span_s,NULL,list_index_s,256);
    if(span_s->wear_count==max_wc_couunt){
        list_add_tail(&span->span_free_list,&central_pool.lb_free_list[list_index_s].free_list); 
    }else{
        list_add(&span->span_free_list,&central_pool.lb_free_list[list_index_s].free_list);
    }   
    return span;
}


static small_block_return_signal_t central_pool_acquire_small_block(int index){
    pthread_mutex_lock(&central_pool.lock);
    small_block_return_signal_t small_block_return_signal;
    span_t *span;
    while(1){
        if(central_pool.free_start+PAGE_SIZE*(index+1)<=central_pool.free_end){
            goto finsh;
            break;
        }else{
            if(central_pool.page_num>=index+1){
                small_block_return_signal.list=central_pool.lb_free_list[0].free_list.next;
                small_block_return_signal.flag=1;   //不连续
                list_head *lh=central_pool.lb_free_list[0].free_list.next;
                for(int i=1;i<=16;i++){
                    lh=lh->next;
                }
                central_pool.lb_free_list[0].free_list.next=lh;
                return small_block_return_signal;                          
            }else{
                if(!list_empty(&central_pool.lb_free_list[index].free_list)){
                    small_block_return_signal.list=central_pool.lb_free_list[index].free_list.next;
                    small_block_return_signal.flag=0;   // 连续
                    list_del(central_pool.lb_free_list[index].free_list.next);
                    return small_block_return_signal;
                }else{
                    for(int i=index+1;i<LARGE_BLOCK_NUM;i++){
                        if(!list_empty(central_pool.lb_free_list[i].free_list.next)){
                            span=span_split(central_pool.lb_free_list[i].free_list.next,i,index);
                            small_block_return_signal.list=&span->span_free_list;
                            small_block_return_signal.flag=0;   // 连续
                            list_del(central_pool.lb_free_list[i].free_list.next);
                            return small_block_return_signal;

                        }
                    }
                }

            }
        }
        central_pool_grow();
    finsh:
        span=central_pool.free_start;
        span->wear_count=0;
        //printf("span addr:%p\n",span);
        central_pool.free_start+=PAGE_SIZE*(index+1);
        small_block_return_signal.list=&(span->span_free_list);
        small_block_return_signal.flag=0;   //  连续
        break;
    }
    pthread_mutex_unlock(&central_pool.lock);
    return small_block_return_signal;

}


span_t *local_cache_replace_foreground(local_cache_t *lc,int index){
    span_t *span;
    if (!list_empty(&(lc->background[index]))){
        span=list_entry(lc->background[index].next,span_t,span_free_list);
        //printf("back:%p span addr:%p  cur_pos:%d\n",lc->background[index].next,span,span->sl_block_t.sm_b.cur_pos);
        list_del(&span->span_free_list);
    }else if(!list_empty(&(lc->free_list))){
        //printf("7\n");
        //printf("11\n");
        span = list_entry(lc->free_list.next,span_t,span_free_list);
        list_del(&span->span_free_list);
        span_init(span,lc,index,index);
    }else{
        //printf("12\n");
        small_block_return_signal_t small_block_return_signal= central_pool_acquire_small_block(Central_TO_Local_Page_Num-1);
        //0代表连续 1代表不连续 
        if(small_block_return_signal.flag==1){
            //printf("9\n");
            span = list_entry(small_block_return_signal.list,span_t,span_free_list);
            span_init(span,lc,index,index);
            list_head *node=small_block_return_signal.list;
            for(int i=1;i<Central_TO_Local_Page_Num;i++){
                list_add_tail(node->next,&lc->free_list);
                node=node->next;
            }
        }else{
            //printf("10\n");
            span = list_entry(small_block_return_signal.list,span_t,span_free_list);
            //printf("----central pool:%p  span:addr:%p---\n",central_pool.free_start,span);
            span_init(span,lc,index,index);
            //printf("index:%d sl_block_t.sm_b.free_block_num:%d wear cout:%d  bitmap:%p\n",index,span->sl_block_t.sm_b.free_block_num,span->wear_count,span->sl_block_t.sm_b.bitmap);
            span_t *span_left=(void *)span+PAGE_SIZE;
            for(int i=1;i<Central_TO_Local_Page_Num;i++){
                list_add_tail(&span_left->span_free_list,&lc->free_list);
                span_left=(void *)span_left+PAGE_SIZE;

            }
            
        }
        int temp=0;
        if(max_wc_couunt-span->wear_count<FIXED_THRESHOLD){
            temp=span->wear_count+FIXED_THRESHOLD;
        }else{
            temp=max_wc_couunt;
        }
       // printf("temp:%d\n",temp);
        if(temp!=lc->local_cache_wc_threshold){
            lc->local_cache_wc_threshold=temp;
            //printf("-----------");
            for(int i=0;i<SMALL_BLOCK_NUM;i++){
                while(!list_empty(&lc->n_avaliable_list[i])){
                    list_head *node=lc->n_avaliable_list[i].next;
                    list_del(node);
                    list_add_tail(node,&lc->background[i]);

                }
                // if(!list_empty(&lc->n_avaliable_list[i])){
                //     list_head *node=lc->n_avaliable_list[i].next;
                //     list_del(node);
                //     list_add_tail(node,&lc->background[i]);
                //     lc->n_avaliable_list[i].next=&lc->n_avaliable_list[i];
                //     lc->n_avaliable_list[i].prev=&lc->n_avaliable_list[i];
                // }
                
            }
        }
    }
    lc->foreground[index]=span;
    return span;
}

//索引从1开始
void *span_clock_allocate(span_t* span,local_cache_t *lc){
    uint64_t bitmap=span->sl_block_t.sm_b.bitmap;
    int16_t cur_pos=span->sl_block_t.sm_b.cur_pos;
    uint16_t last_pos=span->sl_block_t.sm_b.last_pos;
    //printf("span:%p sl_block_t.sm_b.free_block_num:%d wear cout:%d cur_pos:%d  bitmap:%p\n",span,span->sl_block_t.sm_b.free_block_num,span->wear_count,cur_pos,bitmap);
    uint64_t flag;
    uint64_t flag_a=0xFFFFFFFFFFFFFFFF;
    flag=flag_a<<cur_pos;
    //需要从头开始扫描
    if((cur_pos==last_pos)||(bitmap&flag)==0){
        span->wear_count+=1;
        if(span->wear_count>=lc->local_cache_wc_threshold){
            //printf("span addr:%p  wear count:%d\n",span,span->wear_count);
            return NULL;
        }
        cur_pos=0;
    }
    flag=flag_a<<cur_pos;
    bitmap=bitmap&flag;
    bitmap=bitmap&(~bitmap+1);
    if(bitmap&0x00000000FFFFFFFF){
        cur_pos=LOW1BIT((bitmap&0x00000000FFFFFFFF));
    }else{
        bitmap>>=32;
        cur_pos=LOW1BIT((bitmap&0x00000000FFFFFFFF))+32;
    }
   // printf("threadhole6:%d\n",local_cache->local_cache_wc_threshold);
    span->sl_block_t.sm_b.cur_pos=cur_pos;
    // printf("threadhole8:%d\n",local_cache->local_cache_wc_threshold);
     printf("bitmap:%p  cur_pos:%d  free tatol:%d\n",span->sl_block_t.sm_b.bitmap,cur_pos,span->sl_block_t.sm_b.free_block_num);
    span->sl_block_t.sm_b.bitmap&=~((uint64_t)1<<(cur_pos-1));
   // printf("threadhole：%p bitmap:%p threadhole：%p\n",local_cache->local_cache_wc_threshold,span->sl_block_t.sm_b.bitmap,local_cache->local_cache_wc_threshold);
   // printf("threadhole7:%d\n",local_cache->local_cache_wc_threshold);
    return (void *)span+SPAN_SIZE+(cur_pos-1)*(span->block_category+1)*MIN_STORE_UNIT;
}

//达到阈值或者可用内存块用完返回NULL
void *span_alloc_small_block(span_t *span,local_cache_t *lc,int index){
    //printf("4\n");
    void *addr=NULL;
    int sizeof_smalloc_block=(span->block_category+1)*MIN_STORE_UNIT;
    //printf("threadhole4:%d\n",local_cache->local_cache_wc_threshold);
    addr=span_clock_allocate(span,lc);
   // printf("threadhole5:%d\n",local_cache->local_cache_wc_threshold);
    if(addr==NULL){
       // printf("20\n");
        lc->foreground[index]=NULL;
        list_del(&span->span_free_list);
        list_add(&span->span_free_list,&lc->n_avaliable_list[index]);
        return NULL;
    }
    span->sl_block_t.sm_b.free_block_num--;
    if(span->sl_block_t.sm_b.free_block_num==0){
       // printf("21\n");
        lc->foreground[index]=NULL;
    }
    return addr;
}


static void *small_malloc(size_t size){
    //printf("central pool:%p\n",central_pool.free_start);
    void *addr;
    local_cache_t *lc=local_cache;
    span_t *span;
    int index=size/MIN_STORE_UNIT-1;

    // list_head *node=local_cache->background[index].next;
    // while(node!=&local_cache->background[index]){
    //     printf("back:%p node:%p\n",&local_cache->background[index],node);
    //     node=node->next;
    // }

    //printf("local:%p threadhole:%d\n",&local_cache,local_cache->local_cache_wc_threshold);

    if(lc->foreground[index]!=NULL){
        //printf("1\n");
        span=lc->foreground[index];
    }else{
       // printf("2\n");
        span=local_cache_replace_foreground(lc,index);
    }
    addr=span_alloc_small_block(span,lc,index);
    if(addr==NULL){
       // printf("5\n");
        span=local_cache_replace_foreground(lc,index);
       // printf("threadhole1:%d\n",local_cache->local_cache_wc_threshold);
        //printf("span addr:%p\n",span);
        addr=span_alloc_small_block(span,lc,index);
       // printf("threadhole3:%d\n",local_cache->local_cache_wc_threshold);
    }
    printf("addr:%p\n",addr);
    return addr;
}

static span_t* central_pool_acquire_large_block(int index){
    pthread_mutex_lock(&central_pool.lock);
    span_t *span;
    if(central_pool.free_start+PAGE_SIZE*(index+1)<=central_pool.free_end){
        goto finsh;
    }else if(!list_empty(&central_pool.lb_free_list[index].free_list)){
        span=list_entry(&central_pool.lb_free_list[index].free_list.next,span_t,span_free_list);
        list_del(&span->span_free_list);
        return span;
    }else {
        for(int i=index+1;i<LARGE_BLOCK_NUM;i++){
            if(!list_empty(&central_pool.lb_free_list[i].free_list)){
                span=span_split(central_pool.lb_free_list[i].free_list.next,i,index);
                list_del(&span->span_free_list);
                return span;
            }
        }

    }
    central_pool_grow();
    finsh:
        span=central_pool.free_start;
        span->wear_count=0;
        central_pool.free_start+=PAGE_SIZE*(index+1);    
    pthread_mutex_unlock(&central_pool.lock);
    return span;
}


static void* large_malloc(size_t size){
    span_t *span;
    int index=(size+SPAN_SIZE)/PAGE_SIZE;
    span=central_pool_acquire_large_block(index);
    span_init(span,local_cache,index,256);
    span->wear_count+=1;
    return (void *)span+SPAN_SIZE;
}


void *hpnmalloc(size_t size){
    check_init();
    void *addr;
    if(size<=MAX_SMALL_BLOCK_SIZE-SPAN_SIZE){
        addr=small_malloc(size);
    }else if(size<=MAX_LARGE_BLOCK_SIZE-SPAN_SIZE){
        addr=large_malloc(size);
    }
}

static span_t *span_extract_header(void *ptr){
    span_t *span;
    uint64_t unaligned_address=(uint64_t)ptr%PAGE_SIZE;
    span=ptr-unaligned_address;
    return span;
}

static void span_back_to_centralpool(span_t *span){
    int index;
    if(span->block_category!=256){
        index=1;
    }else{
        index=span->sl_block_t.page_num-1;
    }
    if(span->wear_count>max_wc_couunt){
        max_wc_couunt=span->wear_count;
        list_add_tail(&span->span_free_list,&central_pool.lb_free_list[index].free_list);
        central_pool.lb_free_list[index].max_wear_page_index=&span->span_free_list;
        for(int i=0;i<LARGE_BLOCK_NUM;i++){
            if(i==index){
                continue;
            }
            central_pool.lb_free_list[index].max_wear_page_index=NULL;
        }
    }else if(span->wear_count==max_wc_couunt){
        list_add_tail(&span->span_free_list,&central_pool.lb_free_list[index].free_list);
        if(central_pool.lb_free_list[index].max_wear_page_index==NULL){
            central_pool.lb_free_list[index].max_wear_page_index=&span->span_free_list;
        }
    }else{
        if(central_pool.lb_free_list[index].max_wear_page_index==NULL){
            list_add_tail(&span->span_free_list,&central_pool.lb_free_list[index].free_list);
        }else{
            list_add(&span->span_free_list,&central_pool.lb_free_list[index].max_wear_page_index->prev);
        }
    }

}


static void small_free(span_t *span,void *ptr){
    //printf("threadhole2:%d\n",local_cache->local_cache_wc_threshold);
    //printf("31\n");
    local_cache_t *lc=local_cache;
    int size_of_small_block=(span->block_category+1)*MIN_STORE_UNIT;
    span->sl_block_t.sm_b.free_block_num+=1;
    int free_block_num=span->sl_block_t.sm_b.free_block_num;
    //printf("span addr:%p ptr:%p\n",span,ptr);
    if(free_block_num==1){
        //printf("30\n");
        //获取位示图的索引
        int index=((uint64_t)ptr-((uint64_t)span+SPAN_SIZE))/size_of_small_block+1;
        //printf("index:%d\n",index);
        span->sl_block_t.sm_b.bitmap|=(uint64_t)1<<(index-1);
       // printf("----------------------------------------------------span addr:%p----------\n",span);
        if(span->wear_count<lc->local_cache_wc_threshold){
            //printf("span addr:%p\n",span);
            list_add_tail(&span->span_free_list,&lc->background[span->block_category]);
        }
    }else if(free_block_num==span->sl_block_t.sm_b.total_block_num){
        if(span->wear_count>=lc->local_cache_wc_threshold){
            list_del(&span->span_free_list);
            pthread_mutex_lock(&central_pool.lock);
            //返回全局堆
            span_back_to_centralpool(span);
            pthread_mutex_unlock(&central_pool.lock);
        }else{
            //printf("lc->foreground[span->block_category]:%p  &(lc->foreground[span->block_category]):%p\n",lc->foreground[span->block_category],&(lc->foreground[span->block_category]));
            if(lc->foreground[span->block_category]==span){
                 //printf("2\n");
                lc->foreground[span->block_category]=NULL;
            }else{
                //printf("3\n");
                list_del(&span->span_free_list);
            }
            list_add_tail(&span->span_free_list,&lc->free_list);
        }
    }else{
        int index=((uint64_t)ptr-((uint64_t)span+SPAN_SIZE))/size_of_small_block+1;
        //printf("%d\n",index);
        span->sl_block_t.sm_b.bitmap|=(uint64_t)1<<(index-1);
    }
}


static void large_free(span_t *span){
    span_back_to_centralpool(span);
}

void *hpnfree(void *ptr){
    if(ptr==NULL){
        return;
    }
    span_t *span=span_extract_header(ptr);
    //printf("----------free span addr:%p--------\n",span);
    if(span->block_category!=256){
        small_free(span,ptr);
    }else{
        large_free(span);
    }

}
