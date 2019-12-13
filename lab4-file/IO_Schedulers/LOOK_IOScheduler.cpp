//
// Created by CONG YU on 2019/12/13.
//

#include "LOOK_IOScheduler.h"
#include <algorithm>

bool LOOK_IOScheduler::IsPending() {
    return !clockwise.empty() || !counter_clockwise.empty();
}

void LOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    if (request->track_num > dest_track) {
        this->clockwise.push_front(std::move(request));
    } else if (request->track_num < dest_track){
        this->counter_clockwise.push_front(std::move(request));
    } else {
        this->active_queue->push_front(std::move(request));
    }
}

void LOOK_IOScheduler::FetchNext() {
    if (direction > 0 && clockwise.empty()) {
        active_queue = &counter_clockwise;
        direction = -1;
    } else if (direction < 0 && counter_clockwise.empty()) {
        active_queue = &clockwise;
        direction = 1;
    }

    auto it = std::min_element(active_queue->begin(), active_queue->end(), [&](auto const & a, auto const & b) -> bool
    {
        if (std::abs(a->track_num - this->head) == std::abs(b->track_num - this->head)) {
            return a->op_idx < b->op_idx;
        } else {
            return std::abs(a->track_num - this->head) < std::abs(b->track_num - this->head);
        }
    });

    active_io = std::move(*it);
    active_queue->erase(it);

    dest_track = active_io->track_num;

//    if (direction > 0 && wait_queue_1.empty()) {
//        active_queue = &wait_queue_2;
//    } else if (direction < 0 && wait_queue_2.empty()) {
//        active_queue = &wait_queue_1;
//    }
}
