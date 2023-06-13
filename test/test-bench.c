#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "hpnmalloc.h"

#define HPN_MALLOC
#define HPN_FREE

#define S_BLOCK 256
#define ARRAY_SIZE 100005
#define RODOM_BLOCK 15000
#define THREAD_NUM 10


//测试小内存分配释放
void small_malloc_test()
{
    printf("------small memory malloc and free------\n");
    void *addr;
    for (int i = 64; i <= MAX_SMALL_BLOCK_SIZE-SPAN_SIZE; i += MIN_STORE_UNIT)
    {
        addr = hpnmalloc(i);
        printf("addr:%p   size:%d\n", addr, i);
        hpnfree(addr);
        printf("free success!\n");
    }
}

//测试时钟分配算法
void clock_allocate_test(){
     printf("------clock_allocate_test------\n");
     void *addr;
     for (int i = 64; i <= MAX_SMALL_BLOCK_SIZE-SPAN_SIZE; i += MIN_STORE_UNIT){
        int count=0;
        for(int j=1;j<=10000;j++){
            printf("-------------turn:%d----free:%d-----i:%d-------\n",j,count,i);
            addr=hpnmalloc(i);
            if(rand()%2==0){
                hpnfree(addr);
                count++;
            }

        }
     }

        // int count=0;
        // for(int j=1;j<=20000;j++){
        //     printf("-------------turn:%d----free:%d------------\n",j,count);
        //     addr=hpnmalloc(64);
        //     if(rand()%2==0){
        //         hpnfree(addr);
        //         count++;
        //     }

        // }

}

//测试大内存分配释放
void large_malloc_test()
{
    printf("------large memory malloc and free------\n");
    for (int i = PAGE_SIZE-SPAN_SIZE; i <= MAX_LARGE_BLOCK_SIZE-SPAN_SIZE; i += PAGE_SIZE)
    {
        void *addr;
        addr = hpnmalloc(i);
        printf("addr:%p   size:%d\n", addr, i);
        hpnfree(addr);
        printf("free success!\n");
    }
    printf("test done!\n");
}

// //测试超大内存分配释放
// void huge_malloc_test()
// {
//     printf("------large large memory malloc and free------\n");
//     void *addr;
//     for (int i = 1; i <= 100; i++)
//     {
//         addr = hpnmalloc(1024 * 1024 * i);
//         printf("addr:%p   size:%d\n", addr, 1024 * 1024 * i);
//         hpnfree(addr);
//         printf("free success!\n");
//     }
//     printf("test done!\n");
// }


//连续分配10万次，分配后立刻释放，重复10次
void single_thread_test1()
{
    printf("------single thread test1------\n");
    void *addr;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 100000; j++)
        {
            addr = hpnmalloc(S_BLOCK);
            hpnfree(addr);
        }
    }
    printf("test done!\n");
}

//连续分配10万次，然后释放，重复10次
void single_thread_test2()
{
    printf("------single thread test2------\n");
    for (int i = 0; i < 10; i++)
    {
        void *temp[ARRAY_SIZE];
        for (int j = 0; j < 100000; j++)
        {
            temp[j] = hpnmalloc(S_BLOCK);
        }
        for (int j = 0; j < 100000; j++)
        {
            hpnfree(temp[j]);
        }
    }
    printf("test done!\n");
}

//连续分配10万次，分配后随机释放，重复10次
void single_thread_test3()
{
    printf("------single thread test3------\n");
    void *addr;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 100000; j++)
        {
            addr = hpnmalloc(S_BLOCK);
            if (rand() % 2 == 0)
            {
                hpnfree(addr);
            }
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，分配后立刻释放，重复10次
void single_thread_test4()
{
    printf("------single thread test4------\n");
    void *addr;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            addr = hpnmalloc(size);
            hpnfree(addr);
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，然后释放，重复10次
void single_thread_test5()
{
    printf("------single thread test5------\n");
    for (int i = 0; i < 10; i++)
    {
        void *temp[ARRAY_SIZE];
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            temp[j] = hpnmalloc(size);
        }
        for (int j = 0; j < 100000; j++)
        {
            hpnfree(temp[j]);
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，分配后随机释放，重复10次
void single_thread_test6()
{
    printf("------single thread test6------\n");
    void *addr;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            addr = hpnmalloc(size);
            if (rand() % 2 == 0)
            {
                hpnfree(addr);
            }
        }
    }
    printf("test done!\n");
}

void *twork(void *arg)
{
    int flag = *(int *)arg;
    if (flag == 1)
    {
        void *addr;
        for (int j = 0; j < 100000; j++)
        {
            addr = hpnmalloc(64);
            hpnfree(addr);
        }
    }
    else if (flag == 2)
    {
        void *temp[ARRAY_SIZE];
        for (int j = 0; j < 100000; j++)
        {
            temp[j] = hpnmalloc(S_BLOCK);
        }
        for (int j = 0; j < 100000; j++)
        {
            hpnfree(temp[j]);
        }
    }
    else if (flag == 3)
    {
        void *addr;
        for (int j = 0; j < 10000; j++)
        {
            addr = hpnmalloc(64);
            if (rand() % 2 == 0)
            {
                hpnfree(addr);
            }
        }
    }else if(flag == 4){
        void  *addr;
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            addr = hpnmalloc(size);
            hpnfree(addr);
        }
    }else if(flag == 5){
        void *temp[ARRAY_SIZE];
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            temp[j] = hpnmalloc(size);
        }
        for (int j = 0; j < 100000; j++)
        {
            hpnfree(temp[j]);
        }

    }else if(flag == 6){
        void *addr;
        for (int j = 0; j < 100000; j++)
        {
            int size = rand() % RODOM_BLOCK;
            addr = hpnmalloc(size);
            if (rand() % 2 == 0)
            {
                hpnfree(addr);
            }
        }

    }
}

//连续分配10万次，分配后立刻释放
void multi_thread_test1()
{
    printf("------multi thread test1------\n");
    int flag = 1;
    pthread_t tid[100];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    printf("test done!\n");
}

//连续分配10万次，然后释放
void multi_thread_test2()
{
    printf("------multi thread test2------\n");
    pthread_t tid[100];
    int flag = 2;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    printf("test done!\n");
}

//连续分配10万次，分配后随机释放
void multi_thread_test3()
{
    printf("------multi thread test3------\n");
    pthread_t tid[100];
    int flag = 3 ;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，分配后立刻释放
void multi_thread_test4()
{
    printf("------multi thread test4------\n");
    pthread_t tid[100];
    int flag=4;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，然后释放
void multi_thread_test5()
{
    printf("------multi thread test5------\n");
    pthread_t tid[100];
    int flag=4;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    printf("test done!\n");
}

//分配随机大小内存块，连续分配10万次，分配后随机释放
void multi_thread_test6()
{
    printf("------multi thread test6------\n");
    pthread_t tid[100];
    int flag=4;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&tid[i], NULL, &twork, &flag) < 0)
        {
            printf("pthread_create err\n");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    
}

int main()
{
    //small_malloc_test();
    //clock_allocate_test();
    //large_malloc_test();
    //huge_malloc_test();
    //单线程测试
    // single_thread_test1();
    // single_thread_test2();
    // single_thread_test3();
    // single_thread_test4();
    // single_thread_test5();
    // single_thread_test6();
    //多线程测试
    // multi_thread_test1();
    // multi_thread_test2();
     multi_thread_test3();
    // multi_thread_test4();
    // multi_thread_test5();
    // multi_thread_test6();

}