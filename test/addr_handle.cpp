#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <iostream>
//#include "hpnmalloc.h"

using namespace std;

int main(int argc, char **argv) {
    void *p;
    map<size_t, int> mp;
    int total = 0;
    while (fscanf(stdin, "%p", &p) > 0) {
        if((size_t)p%64!=0){
            p=(void *)p-((size_t)p%64);
        }
        if((size_t)p%4096==0){
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
