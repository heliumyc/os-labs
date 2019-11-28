//
// Created by CONG YU on 2019/11/26.
//

#include "Clock_Pager.h"

Frame *Clock_Pager::SelectVictimFrame(Transition& transition) {
    while(true) {
        auto* frame = (*frame_table)[(HAND++) % frame_table_size];
        auto* pte = (*process_list)[frame->pid]->page_table[frame->vpage];
        if (pte->REFERENCED) {
            pte->REFERENCED = 0;
        } else {
            return frame;
        }
    }
}
