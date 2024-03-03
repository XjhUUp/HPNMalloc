#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#define RAW_POOL_START (void *)(0x600000000000)

int main()
{
    void *addr = mmap(RAW_POOL_START, 1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    // strcpy(addr,"阿斯顿发送到发大发部分构成吧的是法国的方式给对方更好abwerewrewrewrc，Hello world，");
    // printf("%s\n", addr);
    memset(addr,0,64);
}
