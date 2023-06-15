#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcnvmalloc.h"

#define TDNUM 256
#define TIMES 100000
#define SIZE 128


//小内存块测试
void small_malloc_test(){
        //printf("------small_malloc_test------\n");
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

int main(int argc, char **argv) {
    small_malloc_test();


}
