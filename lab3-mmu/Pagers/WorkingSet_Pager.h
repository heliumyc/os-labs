//
// Created by CONG YU on 2019/11/26.
//

#ifndef LAB3_MMU_WORKINGSET_PAGER_H
#define LAB3_MMU_WORKINGSET_PAGER_H


#include "../Pager.h"

class WorkingSet_Pager : public Pager {
    Frame* SelectVictimFrame() override;

};


#endif //LAB3_MMU_WORKINGSET_PAGER_H
