//
// Created by CONG YU on 2019/11/26.
//

#include "Aging_Pager.h"
#include "../DataTypes.h"

Frame *Aging_Pager::SelectVictimFrame(Transition& transition) {
    transition.plain_aging_msg = new Plain_Aging_Msg;

    Frame* victim = ( *frame_table )[hand];
    uint32_t min_age = 0xffffffff;

    int i = 0;
    for (; i < frame_table_size; i++) {
        int cur = (i+hand) % frame_table_size;
        auto& frame = ( *frame_table )[cur];
        auto& pte = (*process_list)[frame->pid]->page_table[frame->vpage];

        // first aging
        frame->aging = frame->aging >> 1U;
        frame->aging = pte->REFERENCED? frame->aging | 0x80000000 : frame->aging;
        pte->REFERENCED = 0;

        if (min_age > frame->aging) {
            min_age = frame->aging;
            victim = frame;
        }

        transition.plain_aging_msg->frame_info.emplace_back(frame->fid, frame->aging);
    }

    transition.plain_aging_msg->scan_start = hand;
    transition.plain_aging_msg->scan_end = (i+hand-1) % frame_table_size;
    transition.plain_aging_msg->frame_selected = victim->fid;

    hand = (victim->fid+1) % frame_table_size;
    return victim;
}
