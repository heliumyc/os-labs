//
// Created by CONG YU on 2019/12/13.
//

#ifndef LAB4_FILE_CLOOK_IOSCHEDULER_H
#define LAB4_FILE_CLOOK_IOSCHEDULER_H


#include "../IOScheduler.h"

class CLOOK_IOScheduler : public IOScheduler {
private:
    std::deque<std::unique_ptr<Request>>* active_queue;
    std::deque<std::unique_ptr<Request>>* wait_queue;
    int dest_track = 0; // if active, then it is heading towards this dest
public:
    CLOOK_IOScheduler();
    bool IsPending() override;
    void AddNewIORequest(std::unique_ptr<Request>&& request) override;
    void FetchNext() override;
    ~CLOOK_IOScheduler() override= default;
};


#endif //LAB4_FILE_CLOOK_IOSCHEDULER_H
