#include "hpnmalloc.h"
#include <stdbool.h>

nvm_list_t nvm_list_area;

float weight[10] = {0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};

void init_nvm_area_list()
{
    INIT_LIST_HEAD(&nvm_list_area.nvm_list_head);
}

bool add_nvm_area(nvm_area_t *n_area)
{
    list_add_tail(&n_area->nvm_list_node, &nvm_list_area.nvm_list_head);
    return true;
}

bool del_nvm_area(nvm_area_t *n_area)
{
    list_del(&n_area->nvm_list_node);
    return true;
}

nvm_area_t *make_init_nvm_area(float hardware_mc, float used_mc, uint64_t memory_wear_limit, uint64_t memory_wear_remain, float unit_capacity_mp, float wear_weight)
{
    nvm_area_t *n_area = (nvm_area_t *)malloc(sizeof(nvm_area_t));
    // for(int i=0;i<RAWDELAYN;i++){
    //     n_area.hp_table.read_delay[i]=read_delay[i];
    // }
    // for(int i=0;i<RAWDELAYN;i++){
    //     n_area.hp_table.write_delay[i]=write_delay[i];
    // }
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
    float priority;
    float wear_cost;
    // uint64_t time_cost;
    uint64_t economic_cost;
    nvm_hp_table_t *table = &(n_area->hp_table);
    wear_cost = (float)table->memory_wear_remain / table->unit_capacity_mp; // NVM的剩余磨损次数/单位内存价格
    //economic_cost = table->remain_mc / table->unit_capacity_mp;      // NVM的剩余容量大小/单位内存价格
    priority = n_area->hp_table.wear_weight * wear_cost ;
    n_area->priority = priority;
}

nvm_area_t *select_highest_priority_nvm_area()
{
    nvm_area_t *max_p_nvm = NULL;
    list_head *temp = nvm_list_area.nvm_list_head.next;
    list_head *h = &nvm_list_area.nvm_list_head;
    while (temp != h)
    {
        nvm_area_t *n_area = list_entry(temp, nvm_area_t, nvm_list_node);
        if (max_p_nvm == NULL)
        {
            max_p_nvm = n_area;
        }
        else
        {
            if (max_p_nvm->priority < n_area->priority)
            {
                max_p_nvm = n_area;
            }
        }
        temp = temp->next;
    }
    return max_p_nvm;
}
