//
// Created by CONG YU on 2019/11/7.
//

#ifndef LAB3_MMU_PAGER_H
#define LAB3_MMU_PAGER_H


#include "DataTypes.h"

enum class PagerEnum {FIFO, RANDOM, CLOCK, ESC_NRU, AGING, WORKING_SET};

typedef struct {
    unsigned int PRESENT : 1;
    unsigned int WRITE_PROTECT : 1;
    unsigned int MODIFIED : 1;
    unsigned int REFERENCED : 1;
    unsigned int PAGEDOUT : 1;
    unsigned int FRAME_NUM: 7;
    // following is 20 bits of free allocation
    unsigned int UNK: 20;
} PageTableEntry;

class Pager {
public:
    virtual Frame* SelectVictimFrame() = 0;


};

class PagerFactory {
public:
    static Pager* CreatePager(PagerEnum pager_type);
};

#endif //LAB3_MMU_PAGER_H
