//
// Created by CONG YU on 2019/11/26.
//

#ifndef LAB3_MMU_CLOCK_PAGER_H
#define LAB3_MMU_CLOCK_PAGER_H


#include "../Pager.h"

class Clock_Pager : public Pager{
    unsigned int HAND = 0;
    Frame* SelectVictimFrame() override;
};


#endif //LAB3_MMU_CLOCK_PAGER_H
