#include "hpnmalloc1.h"
#include <stdbool.h>

nvm_list_t nvm_list_area;

float weight[10] = {0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};


bool add_nvm_area(nvm_area_t *n_area)
{
    int index=nvm_list_area.nvm_index;
    nvm_list_area.nvm[index]=n_area;
    nvm_list_area.nvm_index++;
    return true;
}



nvm_area_t *make_init_nvm_area(double hardware_mc, double used_mc, uint64_t memory_wear_limit, uint64_t memory_wear_remain, double unit_capacity_mp, double wear_weight)
{
    nvm_area_t *n_area = (nvm_area_t *)malloc(sizeof(nvm_area_t));
    if (pthread_mutex_init(&n_area->lock, NULL) < 0)
    {
        fprintf(stderr, "fatal error: pthread_mutex_init failed\n");
        exit(-1);
    }
    n_area->hp_table.hardware_mc = hardware_mc;
    n_area->hp_table.used_mc = used_mc;
    n_area->hp_table.memory_wear_limit = memory_wear_limit;
    n_area->hp_table.memory_wear_remain = memory_wear_remain;
    n_area->hp_table.unit_capacity_mp = unit_capacity_mp;
    n_area->hp_table.wear_weight = wear_weight;
    init_priority(n_area);
    add_nvm_area(n_area);
    return n_area;
}


void init_priority(nvm_area_t *n_area)
{
    double priority;
    double wear_cost;
    nvm_hp_table_t *table = &(n_area->hp_table);
    wear_cost = (double)table->memory_wear_remain / (double)table->memory_wear_limit; 
    double memory_write_count=(double)1.0/((double)table->memory_wear_limit-(double)table->memory_wear_remain);
    priority = n_area->hp_table.wear_weight * wear_cost*table->memory_wear_remain*memory_write_count;
    n_area->priority = priority;
}

// void rearrange_nvm(){
//     int index=nvm_list_area.nvm_index;
//     for(int i=0;i<index;i++){
//         for(int j=0;j<index-i-1;j++){
//             if(nvm_list_area.nvm[j]->priority<nvm_list_area.nvm[j+1]->priority){
//                 nvm_area_t *temp=nvm_list_area.nvm[j];
//                 nvm_list_area.nvm[j]=nvm_list_area.nvm[j+1];
//                 nvm_list_area.nvm[j+1]=temp;
//             }
//         }
//     }
// }

