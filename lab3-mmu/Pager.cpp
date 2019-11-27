//
// Created by CONG YU on 2019/11/7.
//

#include "Pager.h"
#include "Pagers/FIFO_Pager.h"
#include "Pagers/Random_Pager.h"
#include "Pagers/Clock_Pager.h"
#include "Pagers/ESC_NRU_Pager.h"
#include "Pagers/WorkingSet_Pager.h"
#include "Pagers/Aging_Pager.h"

Pager* PagerFactory::CreatePager(PagerEnum pager_type) {
    Pager* pager = nullptr;
    switch (pager_type) {
        case PagerEnum::FIFO: pager = new FIFO_Pager(); break;
        case PagerEnum::RANDOM: pager = new Random_Pager(); break;
        case PagerEnum::CLOCK: pager = new Clock_Pager(); break;
        case PagerEnum::ESC_NRU: pager = new ESC_NRU_Pager(); break;
        case PagerEnum::AGING: pager = new Aging_Pager(); break;
        case PagerEnum::WORKING_SET: pager = new WorkingSet_Pager(); break;
        default:
            break;
    }
    return pager;
}

void Pager::SetFrameTable(vector<Frame *> *global_frame_table, int global_frame_table_size){
    frame_table = global_frame_table;
    frame_table_size = global_frame_table_size;
}

void Pager::SetRandomGenerator(MyRandomGenerator *global_random_generator) {
    random_generator = global_random_generator;
}

void Pager::SetProcessList(vector<Process *> *global_process_list) {
    process_list = global_process_list;
}
