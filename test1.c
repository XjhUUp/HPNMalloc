#include <stdio.h>
#include <stdint.h>

static const int debruijn[32] = {
	1, 2, 29, 3, 30, 15, 25, 4, 31, 23, 21, 16, 26, 18, 5, 9,
		32, 28, 14, 24, 22, 20, 17, 8, 27, 13, 19, 7, 12, 6, 11, 10
};
 
#define LOW1BIT(X) (debruijn[((unsigned int)(X * 0x077CB531U)) >> 27])

int main(){
uint64_t a=0x00000080;
//printf("%u\n",a&0x00000000FFFFFFFF);
printf("%d\n",LOW1BIT((a)));



}

// #include <stdio.h>
 
// int main()
// {
// 	unsigned int k = 1 ,t = 1 ,s;
// 	int i;
// 	unsigned int bitmap[64];
// 	unsigned int pow2[64];
 
// 	memset(bitmap ,0 ,sizeof(bitmap));
 
// 	for(i = 0 ;i < 64 ;i ++)
// 		pow2[i] = 1<<i;
 
// 	while(k != 0xFFFFFFFFFFFFFFFF) {
// 		for(i = 31 ; i >= 0 ;i --) {
// 			s = (pow2[i] * k) >> 59;
// 			if(bitmap[s] < t) {
// 				bitmap[s] = t;
// 			} else {
// 				break;
// 			}
// 		}
 
// 		if(i < 0) {
// 			printf("0x%XU\n" ,k);
// 		//	break;
// 		}
 
// 		++ k;
// 		++ t;
// 	}
 
// 	return 0;
// }
 
//  #include <stdio.h>
//  #include <stdint.h>
//  #include <stdbool.h>
 
//  int main(){
//     uint64_t pow2[64];
//     uint64_t k=1;
//     uint64_t s;
//     for(int i=0;i<64;i++){
//         pow2[i]=(uint64_t)1<<i;
//     }
//     while(k!=0xFFFFFFFFFFFFFFFF){
//         bool visited[256];
//         memset(visited,0,sizeof(visited));
//         bool flag=true;
//         for(int i=63;i>=0;i--){
//             s=(pow2[i]*k)>>56;
//             if(visited[s]==1){
//                 flag=false;
//                 break;
//             }else{
//                 visited[s]=1;
//             }
//         }
//         if(flag==true){
//             printf("val:%p\n",k);
//         }
//         ++k;
//         //printf("val:%p\n",k);
        

//     }
//  }
 