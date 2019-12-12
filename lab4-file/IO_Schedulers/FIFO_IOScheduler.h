//
// Created by CONG YU on 2019/12/10.
//

#ifndef LAB4_FILE_FIFO_IOSCHEDULER_H
#define LAB4_FILE_FIFO_IOSCHEDULER_H

#include "../IOScheduler.h"
#include <memory>

using namespace std;
class FIFO_IOScheduler : public IOScheduler {
private:
    queue<unique_ptr<Request>> wait_queue;
public:
    FIFO_IOScheduler()= default;;
    bool IsPending() override;
    void AddNewIORequest(std::unique_ptr<Request>&& request) override;
    void FetchNextAndStartNewIO() override;
    void MoveForward() override;
    ~FIFO_IOScheduler() override= default;;
};


#endif //LAB4_FILE_FIFO_IOSCHEDULER_H
