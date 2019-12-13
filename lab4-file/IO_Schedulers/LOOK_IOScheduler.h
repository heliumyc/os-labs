//
// Created by CONG YU on 2019/12/13.
//

#ifndef LAB4_FILE_LOOK_IOSCHEDULER_H
#define LAB4_FILE_LOOK_IOSCHEDULER_H


#include "../IOScheduler.h"
#include <deque>

class LOOK_IOScheduler : public IOScheduler{
private:
    std::deque<std::unique_ptr<Request>> clockwise{};
    std::deque<std::unique_ptr<Request>> counter_clockwise{};
    std::deque<std::unique_ptr<Request>>* wait_queue = &clockwise;
    int dest_track = 0; // if active, then it is heading towards this dest
public:
    LOOK_IOScheduler()= default;
    bool IsPending() override;
    void AddNewIORequest(std::unique_ptr<Request>&& request) override;
    void FetchNext() override;
    ~LOOK_IOScheduler() override= default;
};

#endif //LAB4_FILE_LOOK_IOSCHEDULER_H
