#include <stdio.h>
#include <stdint.h>


//((size_t)p%CHUNK_SIZE-96)%64!=0
int main(){
    //printf("3\n");
    uint64_t a=105553116270208;
    size_t d=105553116270240;
    uint64_t b=4096*16+96;
    uint64_t c=(a%b-96)%64;
    //printf("val:%llu\n",c);
    if((a%(4096*16+96)-96)%64!=0){
        printf("1\n");
    }
    if((d%(4096*16+96)-96)%64!=0){
        printf("2\n");
    }

    //uint64_t c=a%b;
    //printf("%lld\n",c);
    // if(a%b==0){
    //     printf("1\n");
    // }else{
    //     printf("2\n");
    // }
    // printf("4\n");


}