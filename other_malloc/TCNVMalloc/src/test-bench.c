#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcnvmalloc.h"

#define TDNUM 256
#define TIMES 100000
#define SIZE 128
#define THREAD_NUM 5


//小内存块测试
void small_malloc_test(){
        printf("------small_malloc_test------\n");
        void *addr;
        int count=0;
        for(int j=1;j<=20000;j++){ 
            //printf("-------------turn:%d----free:%d------------\n",j,count);
            addr=wa_malloc(64);
            //printf("addr:%p\n",addr);
            memset(addr,0,64);
            if(rand()%2==0){
                wa_free(addr);
                count++;
            }
        }
}

void *twork(void *arg)
{
    int flag = *(int *)arg;
    if (flag == 1)
    {
        void *addr;
        for (int j = 0; j < 1000; j++)
        {
            addr = wa_malloc(64);
            wa_free(addr);
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

int main(int argc, char **argv) {
    //small_malloc_test();
    multi_thread_test1();
    printf("end\n");


}
