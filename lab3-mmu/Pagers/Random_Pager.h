//
// Created by CONG YU on 2019/11/26.
//

#ifndef LAB3_MMU_RANDOM_PAGER_H
#define LAB3_MMU_RANDOM_PAGER_H


#include "../DataTypes.h"
#include "../Pager.h"

class Random_Pager : public Pager  {
    Frame* SelectVictimFrame() override;
};

#endif //LAB3_MMU_RANDOM_PAGER_H
