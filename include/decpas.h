#ifndef DECPAS_H_
#define DECPAS_H_

#include <stdint.h>

#define RAWDELAYN   16

typedef struct nvm_hardware_parameter_table nvm_hp_table;
typedef struct nvm_memory_allocate_area nvm_area;




struct nvm_hardware_parameter_table{
    uint16_t read_delay[RAWDELAYN]; //读延时，单位ns
    uint16_t write_delay[RAWDELAYN];   //写延时，单位ns
    uint32_t hardware_mc; //硬件容量大小，单位MB
    uint32_t remain_mc; //剩余容量大小，单位MB
    uint64_t memory_wear_limit; //  内存磨损上限，单位次数
    float    unit_capacity_mp;  //单位容量内存价格
};

struct nvm_memory_allocate_area{
    nvm_hp_table hp_table;
    void *addr;
};


#endif