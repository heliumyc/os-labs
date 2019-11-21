//
// Created by CONG YU on 2019/11/7.
//

#ifndef LAB3_MMU_PAGER_H
#define LAB3_MMU_PAGER_H

#include "DataTypes.h"

using namespace std;

enum class PagerEnum {FIFO, RANDOM, CLOCK, ESC_NRU, AGING, WORKING_SET};

class Pager {

public:
    vector<Frame*>* frame_table;
    virtual Frame* SelectVictimFrame() = 0;
    void SetFrameTable(vector<Frame*>* global_frame_table);
};

class PagerFactory {
public:
    static Pager* CreatePager(PagerEnum pager_type);
};

#endif //LAB3_MMU_PAGER_H
