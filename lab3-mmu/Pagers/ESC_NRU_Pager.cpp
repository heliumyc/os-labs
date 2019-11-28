//
// Created by CONG YU on 2019/11/26.
//

#include "ESC_NRU_Pager.h"

Frame *ESC_NRU_Pager::SelectVictimFrame(Transition& transition) {
    transition.esc_nru_msg = new ESC_NRU_Aging_Msg;
    transition.esc_nru_msg->hand = hand;

    Frame* candidates[4];
    for (auto& c: candidates) {
        c = nullptr;
    }

    Frame* victim = ( *frame_table )[hand];
    // scan
    int scan_cnt = 0;
    while (scan_cnt < frame_table_size) {
        scan_cnt++;
        int cur = (hand + scan_cnt - 1) % frame_table_size;
        auto& frame = ( *frame_table )[cur];
        auto& pte = (*process_list)[frame->pid]->page_table[frame->vpage];
        auto prio = 2U * pte->REFERENCED + pte->MODIFIED;

        if (candidates[prio] == nullptr) {
            candidates[prio] = frame;
        }
        if (prio == 0) {
            break;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (candidates[i] != nullptr) {
            victim = candidates[i];
            transition.esc_nru_msg->lowest = i;
            transition.esc_nru_msg->victim_frame = candidates[i]->fid;
            break;
        }
    }

    if (( transition.instruction_idx + 1  ) >= (50 + timer)) {
        // your time is up, face the rage of pager !
        // reset all the R bit
//        timer = 50 *  (int) ((transition.instruction_idx+1) / 50);
        timer = transition.instruction_idx + 1;

        // clear R
        for (auto& frame: *frame_table) {
            auto& pte = ( *process_list )[frame->pid]->page_table[frame->vpage];
            pte->REFERENCED = 0;
        }
        transition.esc_nru_msg->reset = true;
    }

    transition.esc_nru_msg->scan_num = scan_cnt;
    hand = (victim->fid+1) % frame_table_size;
    return victim;
}
