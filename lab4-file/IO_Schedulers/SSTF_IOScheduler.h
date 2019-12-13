//
// Created by CONG YU on 2019/12/12.
//

#ifndef LAB4_FILE_SSTF_IOSCHEDULER_H
#define LAB4_FILE_SSTF_IOSCHEDULER_H


#include <memory>
#include "../IOScheduler.h"

class SSTF_IOScheduler : public IOScheduler {
private:
    std::vector<std::unique_ptr<Request>> wait_queue;
public:
    SSTF_IOScheduler()= default;
    bool IsPending() override;
    void AddNewIORequest(std::unique_ptr<Request>& request) override;
    void FetchNext() override;
    ~SSTF_IOScheduler() override= default;
};


#endif //LAB4_FILE_SSTF_IOSCHEDULER_H
