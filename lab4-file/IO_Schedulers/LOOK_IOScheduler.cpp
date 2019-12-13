//
// Created by CONG YU on 2019/12/13.
//

#include "LOOK_IOScheduler.h"

bool LOOK_IOScheduler::IsPending() {
    return !clockwise.empty() || !counter_clockwise.empty();
}

void LOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &request) {
    if (request->track_num > dest_track) {
        this->clockwise.push_front(std::move(request));
        cout << this->clockwise.size() << endl;
    } else if (request->track_num < dest_track){
        this->counter_clockwise.push_front(std::move(request));
        cout << this->counter_clockwise.size() << endl;
    } else {
        if (direction > 0) {
            clockwise.push_front(std::move(request));
        } else {
            counter_clockwise.push_front(std::move(request));
        }
    }
}

void LOOK_IOScheduler::FetchNext() {
    if (direction > 0 && clockwise.empty()) {
        wait_queue = &counter_clockwise;
    } else if (direction < 0 && counter_clockwise.empty()) {
        wait_queue = &clockwise;
    }

    auto it = std::min_element(wait_queue->begin(), wait_queue->end(), [&](auto const & a, auto const & b) -> bool
    {
        if (std::abs(a->track_num - this->head) == std::abs(b->track_num - this->head)) {
            return a->op_idx > b->op_idx;
        } else {
            return std::abs(a->track_num - this->head) > std::abs(b->track_num - this->head);
        }
    });

    active_io = std::move(*it);
    wait_queue->erase(it);

    dest_track = active_io->track_num;
    wait_queue->pop_back();

//    if (direction > 0 && clockwise.empty()) {
//        wait_queue = &counter_clockwise;
//    } else if (direction < 0 && counter_clockwise.empty()) {
//        wait_queue = &clockwise;
//    }
}
