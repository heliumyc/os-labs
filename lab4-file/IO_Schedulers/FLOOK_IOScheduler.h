//
// Created by CONG YU on 2019/12/13.
//

#ifndef LAB4_FILE_FLOOK_IOSCHEDULER_H
#define LAB4_FILE_FLOOK_IOSCHEDULER_H


#include <deque>
#include "../CommonTypes.h"
#include "../IOScheduler.h"

class FLOOK_IOScheduler : public IOScheduler{
private:
    std::deque<std::unique_ptr<Request>> add_queue{};
    std::deque<std::unique_ptr<Request>> clockwise{};
    std::deque<std::unique_ptr<Request>> counter_clockwise{};
    std::deque<std::unique_ptr<Request>>* active_queue = &clockwise;
    void AllocateFromAddQueue();
    int dest_track = 0; // if active, then it is heading towards this dest
public:
    FLOOK_IOScheduler() = default;
    bool IsPending() override;
    void AddNewIORequest(std::unique_ptr<Request>&& request) override;
    void FetchNext() override;
    ~FLOOK_IOScheduler() override= default;
};


#endif //LAB4_FILE_FLOOK_IOSCHEDULER_H
