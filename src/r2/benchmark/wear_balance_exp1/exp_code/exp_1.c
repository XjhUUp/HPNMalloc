/*
    文件作用：基本测试集
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pthread.h"
#ifdef HPNMALLOC
#include "hpnmalloc.h"
#elif NVMALLOC
#include "nvmalloc.h"
#elif WAMALLOC
#include "tcnvmalloc.h"
#elif HPNMALLOC1
#include "hpnmalloc1.h"
#endif



int g_total_times = 100000;
int g_tdnum = 1;
int g_threadtime[100];
int g_threadid[100];

int flag;
int BLKSIZE;

void *twork(void *arg)
{
    srand(time(NULL));
    int j = 0;

    char *addr = NULL;
    size_t i;
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (i = 0; i < flag; i++)
    {

        int size = BLKSIZE;
        //printf("i:%d  size:%d\n",i,size);

        if (size == 0)
        {
            continue;
        }
#ifdef NVMALLOC
        addr = (char *)nvmalloc(size);
#elif HPNMALLOC
        addr = (char *)hpnmalloc(size);
#elif WAMALLOC
        addr = (char *)wa_malloc(size);
#elif HPNMALLOC1
        addr = (char *)hpnmalloc1(size);
#endif
        memset(addr, 1, size);
        if (rand() % 2 == 0)
        {
            j = 0;
#ifdef HPNMALLOC
            hpnfree(addr);
#elif NVMALLOC
            nvfree(addr);
#elif WAMALLOC
            wa_free(addr);
#elif HPNMALLOC1
            hpnfree1(addr);
#endif
        }
        // else{
        //     j++;
        // }
    }

    gettimeofday(&end, NULL);
    int time_in_us = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec); // 单位是微秒
    g_threadtime[*(int *)arg] = time_in_us;
    return NULL;
}

int main(int argc, char *argv[])
{

#ifdef NVMALLOC
    nvmalloc_init(1024, 100);
#endif
    pthread_t tid[1000];
    int i, rc;
    srand(time(NULL));
    flag = atoi(argv[1]);
    BLKSIZE=atoi(argv[2]);
    for (i = 0; i < g_tdnum; i++)
    {
        g_threadid[i] = i;
        if (pthread_create(&tid[i], NULL, &twork, (void *)&g_threadid[i]) < 0)
        {
            printf("pthread_create err\n");
        }
    }

    for (i = 0; i < g_tdnum; i++)
    {
        if (pthread_join(tid[i], NULL) < 0)
        {
            printf("pthread_join err\n");
        }
    }
    float time_in_us = 0;
    for (i = 0; i < g_tdnum; i++)
    {
        // 分配一次的平均时间
        time_in_us += g_threadtime[i] / ((double)(g_tdnum * g_total_times));
    }
    // printf("%lf", time_in_us);
    return 0;
}
