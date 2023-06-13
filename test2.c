#include <stdio.h>
#include <stdint.h>

int main(){
    uint64_t k=1;
    for(int i=0;i<64;i++){
        printf("i:%d  %llu\n",i,k<<i);
    }
}