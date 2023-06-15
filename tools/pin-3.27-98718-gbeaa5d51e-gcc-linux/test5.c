#include <stdio.h>
#include <sys/mman.h>

int main(){
    void * addr= mmap(0, 8, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    *(int *)addr=1;
    *(int *)addr=2;
    int *addr1=(int *)addr+1;
    *(int *)addr1=3;
    printf("%p\n",addr);
    return 0;

}
