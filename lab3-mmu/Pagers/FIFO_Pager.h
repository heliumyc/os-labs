//
// Created by CONG YU on 2019/11/10.
//

#ifndef LAB3_MMU_FIFO_PAGER_H
#define LAB3_MMU_FIFO_PAGER_H


#include "../Pager.h"

class FIFO_Pager : public Pager {
    Frame* SelectVictimFrame() override;
};


#endif //LAB3_MMU_FIFO_PAGER_H
