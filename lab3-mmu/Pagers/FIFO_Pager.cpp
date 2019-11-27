//
// Created by CONG YU on 2019/11/10.
//

#include "FIFO_Pager.h"

Frame* FIFO_Pager::SelectVictimFrame() {
    Frame* victim = ( *this->frame_table )[head];
    head = ( head + 1 ) % this->frame_table->size();
    return victim;
}
