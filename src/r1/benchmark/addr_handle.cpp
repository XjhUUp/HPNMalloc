#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <iostream>
#include "hpnmalloc.h"

using namespace std;

void *p;

void func(int flag)
{
    if (flag == 1)
    {
        uint64_t nvm_count = 0;
        uint64_t dram_count = 0;
        while (fscanf(stdin, "%p", &p) > 0)
        {
            if ((p >= RAW_POOL_START3 && (uint64_t)p < 0x631000000000) || (p >= RAW_POOL_START2 && (uint64_t)p < 0x621000000000) || (p >= RAW_POOL_START1 && (uint64_t)p < 0x611000000000) || (p >= RAW_POOL_START0 && (uint64_t)p < 0x601000000000))
            {
                nvm_count++;
            }
            else
            {
                dram_count++;
            }
        }
        double ratio = (double)(dram_count) / (double)(nvm_count + dram_count);
        cout << nvm_count << " " << dram_count << " " << nvm_count + dram_count << " " << ratio << endl;
    }

    else if (flag == 2)
    {
        map<uint64_t, int> mp; // key是地址 value是磨损次数
        while (fscanf(stdin, "%p", &p) > 0)
        {
            void *temp;
            temp = p;
            
            if ((uint64_t)temp % 64 != 0)
            {
                temp = (void *)temp - ((uint64_t)temp % 64);
            }
            mp[(uint64_t)temp]++;
        }
        map<int, int> m; // key是磨损次数 value是block数量
        for (auto it : mp)
        {
            int temp = it.second;
            m[temp]++;
        }
        for (auto it : m)
        {
            cout << it.first << " " << it.second << endl;
        }
    }
    else if (flag == 3)
    {
        uint64_t total = 0;
        uint64_t NVM0_wc = 0;
        uint64_t NVM1_wc = 0;
        uint64_t NVM2_wc = 0;
        uint64_t NVM3_wc = 0;
        while (fscanf(stdin, "%p", &p) > 0)
        {
            if (p >= RAW_POOL_START3 && (uint64_t)p < 0x631000000000)
            {
                NVM3_wc += 1;
            }
            else if (p >= RAW_POOL_START2 && (uint64_t)p < 0x621000000000)
            {
                NVM2_wc += 1;
            }
            else if (p >= RAW_POOL_START1 && (uint64_t)p < 0x611000000000)
            {
                NVM1_wc += 1;
            }
            else if (p >= RAW_POOL_START0 && (uint64_t)p < 0x601000000000)
            {
                NVM0_wc += 1;
            }
        }
        double w1 = (double)NVM0_wc / (double)1000000000000;
        double w2 = (double)NVM1_wc / (double)1000000000000;
        double w3 = (double)NVM2_wc / (double)100000000;
        double w4 = (double)NVM3_wc / (double)10000000000;
        // cout<<NVM0_wc<<" "<<NVM1_wc<<" "<<NVM2_wc<<" "<<NVM3_wc<<endl;
        // cout<< fixed << setprecision(10)<<w1<<" "<<w2<<" "<<w3<<" "<<w4<<endl;
        double sum=w1+w2+w3+w4;
        double avg=sum/4;
        double fc=0;
        fc+=(w1-avg)*(w1-avg);
        fc+=(w2-avg)*(w2-avg);
        fc+=(w3-avg)*(w3-avg);
        fc+=(w4-avg)*(w4-avg);
        fc=fc/4;
         cout << NVM0_wc << " " << NVM1_wc << " " << NVM2_wc << " " << NVM3_wc << " " << w1 << " " << w2 << " " << w3 << " " << w4 <<" "<<fc<< endl;
    }
}

int main(int argc, char *argv[])
{
    int flag = atoi(argv[1]);
    func(flag);

    return 0;
}

