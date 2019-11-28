//
// Created by CONG YU on 2019/11/26.
//

#include "Random_Pager.h"

Frame *Random_Pager::SelectVictimFrame(Transition& transition) {
    int random_victim_index = random_generator->GetRandomInt() % frame_table_size;
    return ( *frame_table )[random_victim_index];
}
