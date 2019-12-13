//
// Created by CONG YU on 2019/12/13.
//

#include "CLOOK_IOScheduler.h"
#include <algorithm>

bool CLOOK_IOScheduler::IsPending() {
    return !active_queue->empty() || !wait_queue->empty();
}

void CLOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    if (request->track_num >= dest_track) {
         active_queue->push_front(std::move(request));
    } else if (request->track_num < dest_track){
         wait_queue->push_front(std::move(request));
    }
}

void CLOOK_IOScheduler::FetchNext() {
    if (active_queue->empty()) {
        // reverse to the very leftmost and start
        std::swap(active_queue, wait_queue);
    }

    auto it = std::min_element(active_queue->begin(), active_queue->end(), [&](auto const & a, auto const & b) -> bool
    {
        if (a->track_num == b->track_num) {
            return a->op_idx < b->op_idx;
        } else {
            return a->track_num < b->track_num;
        }
    });
    active_io = std::move(*it);
    active_queue->erase(it);
    dest_track = active_io->track_num;
    this->direction = this->active_io->track_num > this->head? 1 : this->active_io->track_num < this->head? -1 : 0;
}

CLOOK_IOScheduler::CLOOK_IOScheduler() {
    // may cause memory leak!!!!!!
//    this->active_queue = new std::deque<std::unique_ptr<Request>>();
//    this->wait_queue = new std::deque<std::unique_ptr<Request>>();
    this->active_queue = &run_queue_1;
    this->wait_queue = &run_queue_2;
}
