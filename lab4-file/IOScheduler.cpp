//
// Created by CONG YU on 2019/12/10.
//

#include "IOScheduler.h"
#include "IO_Schedulers/FIFO_IOScheduler.h"
#include "IO_Schedulers/CLOOK_IOScheduler.h"
#include "IO_Schedulers/LOOK_IOScheduler.h"
#include "IO_Schedulers/SSTF_IOScheduler.h"
#include "IO_Schedulers/FLOOK_IOScheduler.h"

IOScheduler *IOSchedulerFactory::CreatePager(IOSchedType io_sched_type) {
    IOScheduler* scheduler = nullptr;
    switch (io_sched_type) {
        case IOSchedType::FIFO: scheduler = new FIFO_IOScheduler(); break;
        case IOSchedType::CLOOK: scheduler = new CLOOK_IOScheduler(); break;
        case IOSchedType::SSTF: scheduler = new SSTF_IOScheduler(); break;
        case IOSchedType::FLOOK: scheduler = new FLOOK_IOScheduler(); break;
        case IOSchedType::LOOK: scheduler = new LOOK_IOScheduler(); break;
        default:
            break;
    }
    return scheduler;
}
