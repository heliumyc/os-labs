//
// Created by CONG YU on 2019/12/12.
//

#include "SSTF_IOScheduler.h"
#include <algorithm>
bool SSTF_IOScheduler::IsPending() {
    return !this->wait_queue.empty();
}

void SSTF_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    wait_queue.push_back(std::move(request));
}

void SSTF_IOScheduler::FetchNext() {
    // this is a very stupid impl that it sorts everything every time
    auto it = std::min_element(wait_queue.begin(), wait_queue.end(), [&](auto const & a, auto const & b) -> bool
    {
        if (std::abs(a->track_num - this->head) == std::abs(b->track_num - this->head)) {
            return a->op_idx < b->op_idx;
        } else {
            return std::abs(a->track_num - this->head) < std::abs(b->track_num - this->head);
        }
    });
    active_io = std::move(*it);
    wait_queue.erase(it);
    this->direction = this->active_io->track_num > this->head? 1 : this->active_io->track_num < this->head? -1 : 0;
}

