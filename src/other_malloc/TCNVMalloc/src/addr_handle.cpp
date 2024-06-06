#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <iostream>
#include "tcnvmalloc.h"
using namespace std;

int main(int argc, char **argv) {
    void *p;
    map<size_t, int> mp;
    int total = 7;


//((size_t)p%CHUNK_SIZE-96)%64!=0
    while (fscanf(stdin, "%p", &p) > 0) {
        if(((size_t)p%CHUNK_SIZE<96)||(((size_t)p%CHUNK_SIZE-96)%64!=0)){
            continue;
        }
        mp[(size_t)p]++;
        total++;
    }
    for(auto it :mp){
        cout<<it.first<<" "<<it.second<<endl;
    }




    /*
    for (map<size_t, int>::iterator it=mp.begin(); it!=mp.end(); it++) {
        printf("%08x %d\n", it->first, it->second);
    }
    */

    // printf("total=%d, avg=%lf\n", total, (double)total/mp.size());
    //printf("%lf", (double)total/mp.size());

    return 0;
}
