//
// Created by CONG YU on 2019/11/26.
//

#include "WorkingSet_Pager.h"

Frame *WorkingSet_Pager::SelectVictimFrame(Transition& transition) {
    transition.ws_aging_msg = new WS_Aging_Msg;
    Frame* victim = ( *frame_table )[0];
    int min_instr = INT32_MAX;
    unsigned int min_R_bit = 1;

    for (int i = 0; i < frame_table_size; ++i) {
        int cur = (i+hand) % frame_table_size;
        auto& frame = (*frame_table)[cur];
        auto& pte = (*process_list)[frame->pid]->page_table[frame->vpage];

        if (( min_R_bit == pte->REFERENCED && min_instr > frame->last_use)
                || (min_R_bit && !pte->REFERENCED)) {
            min_R_bit = pte->REFERENCED;
            min_instr = frame->last_use;
            victim = frame;
        }

        transition.ws_aging_msg->frame_info.emplace_back(frame->fid, (int) pte->REFERENCED, frame->pid, frame->vpage, frame->last_use);

        if (transition.instruction_idx - frame->last_use >= 50 && !pte->REFERENCED) {
//            pte->REFERENCED = 0; // it does not matter
            victim = frame;
            transition.ws_aging_msg->early_stop = true;
            transition.ws_aging_msg->scan_cnt = i+1;
            break;
        }
        if (pte->REFERENCED) {
            frame->last_use = transition.instruction_idx;
            pte->REFERENCED = 0;
        }
    }

    transition.ws_aging_msg->scan_start = hand;
    transition.ws_aging_msg->scan_end = (hand-1 + frame_table_size )%frame_table_size;
    transition.ws_aging_msg->frame_selected = victim->fid;

    hand = (victim->fid + 1) % frame_table_size;
    return victim;
}
