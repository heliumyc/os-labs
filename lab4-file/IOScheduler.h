//
// Created by CONG YU on 2019/12/10.
//

#ifndef LAB4_FILE_IOSCHEDULER_H
#define LAB4_FILE_IOSCHEDULER_H

#include <queue>
#include "CommonTypes.h"

enum class IOSchedType {FIFO, SSTF, LOOK, CLOOK, FLOOK};

class IOScheduler {

};

class IOSchedulerFactory {
public:
    static IOScheduler* CreatePager(IOSchedType io_sched_type);
};

#endif //LAB4_FILE_IOSCHEDULER_H
