//
// Created by CONG YU on 2019/12/13.
//

#include "FLOOK_IOScheduler.h"
#include <algorithm>

bool FLOOK_IOScheduler::IsPending() {
    return !clockwise.empty() || !counter_clockwise.empty() || !add_queue.empty();
}

void FLOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    // add only applies to add queue
    add_queue.push_front(std::move(request));
}

void FLOOK_IOScheduler::FetchNext() {
    if (clockwise.empty() && counter_clockwise.empty()) {
        // add queue is guaranteed to be non-empty, so after allocated, this will not be allocated again
        AllocateFromAddQueue();
        return FetchNext();
    } else if (direction > 0 && clockwise.empty()) {
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
}

void FLOOK_IOScheduler::AllocateFromAddQueue() {
    // dissect the add queue!
    while (!add_queue.empty()) {
        auto& it = add_queue.front();
        if (it->track_num > head) {
            clockwise.push_back(std::move(it));
        } else if (it->track_num < head) {
            counter_clockwise.push_back(std::move(it));
        } else {
            // special case!
            active_queue->push_back(std::move(it));
        }
        add_queue.pop_front();
    }
}
