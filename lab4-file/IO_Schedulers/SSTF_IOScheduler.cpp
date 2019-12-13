//
// Created by CONG YU on 2019/12/12.
//

#include "SSTF_IOScheduler.h"

void SSTF_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    wait_queue.push_back(std::move(request));
}

void SSTF_IOScheduler::FetchNext() {

}

bool SSTF_IOScheduler::IsPending() {
    return false;
}
