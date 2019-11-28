//
// Created by CONG YU on 2019/11/26.
//

#ifndef LAB3_MMU_ESC_NRU_PAGER_H
#define LAB3_MMU_ESC_NRU_PAGER_H


#include "../Pager.h"

class ESC_NRU_Pager : public Pager{
    int timer = 0;
    int hand = 0;
    Frame* SelectVictimFrame(Transition& transition) override;
};


#endif //LAB3_MMU_ESC_NRU_PAGER_H
