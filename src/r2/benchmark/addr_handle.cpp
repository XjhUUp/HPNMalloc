#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <iostream>
#include <iomanip>
#include "hpnmalloc.h"

using namespace std;

void *p;

void func(int flag)
{
    if (flag == 1)
    {
        int nvm_count = 0;
        int dram_count = 0;
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
        int count0=0;
        while (fscanf(stdin, "%p", &p) > 0)
        {
            void *temp;
            temp = p;
            if ((uint64_t)temp % 64 != 0)
            {
                temp = (void *)temp - ((uint64_t)temp % 64);
            }

            if(mp.count((uint64_t)temp)==0){
                count0++;
            }

            mp[(uint64_t)temp]++;
        }
        map<int, int> m; // key是磨损次数 value是block数量
        for (auto it : mp)
        {
            int temp = it.second;
            m[temp]++;
        }

        double sum = 0;
        double num = 0;
        double avg = 0;
        for (auto it : m)
        {
            //cout << it.first << " " << it.second << endl;
            sum += it.first*it.second;
            num+=it.second;
        }
        avg = sum / num;
        double fc = 0;
        for (auto it : m)
        {
            fc += (it.first - avg) * (it.first - avg)*it.second;
        }
        fc = fc / num;
        cout << fc << " "<<avg<<" "<<count0<< endl;
    }
    
    
    else if (flag == 3)
    {
        map<uint64_t, int> mp; // key是地址 value是磨损次数
        int count0=0;
        while (fscanf(stdin, "%p", &p) > 0)
        {
            void *temp;
            temp = p;

            int chunk_size=16*4096+96;
            if((uint64_t)temp%chunk_size<96){
                continue;
            }

            if ((uint64_t)temp % 64 != 0)
            {
                temp = (void *)temp - ((uint64_t)temp % 64);
            }
            
            if(mp.count((uint64_t)temp)==0){
                count0++;
            }
            mp[(uint64_t)temp]++;
        }
        map<int, int> m; // key是磨损次数 value是block数量
        for (auto it : mp)
        {
            int temp = it.second;
            m[temp]++;
        }

        double sum = 0;
        double num = 0;
        double avg = 0;
        for (auto it : m)
        {
            //cout << it.first << " " << it.second << endl;
            sum += it.first*it.second;
            num+=it.second;
        }
        avg = sum / num;
        double fc = 0;
        for (auto it : m)
        {
            fc += (it.first - avg) * (it.first - avg)*it.second;
        }
        fc = fc / num;
        cout << fc <<" "<<avg<<" "<<count0<< endl;
    }
    else if (flag == 4)
    {
        int total = 0;
        int NVM0_wc = 0;
        int NVM1_wc = 0;
        int NVM2_wc = 0;
        int NVM3_wc = 0;
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
        cout << w1 << " " << w2 << " " << w3 << " " << w4 << endl;
    }
}

int main(int argc, char *argv[])
{
    int flag = atoi(argv[1]);
    func(flag);

    return 0;
}
