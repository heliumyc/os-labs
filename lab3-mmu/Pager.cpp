//
// Created by CONG YU on 2019/11/7.
//

#include "Pager.h"
#include "Pagers/FIFO_Pager.h"

Pager *PagerFactory::CreatePager(PagerEnum pager_type) {
    Pager* pager;
    switch (pager_type) {
        case PagerEnum::FIFO: pager = new FIFO_Pager();
            break;
        default:
            break;
    }
    return pager;
}
