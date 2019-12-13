//
// Created by CONG YU on 2019/12/13.
//

#include "CLOOK_IOScheduler.h"

bool CLOOK_IOScheduler::IsPending() {
    return false;
}

void CLOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    if (request->track_num > dest_track) {
        this->wait_queue_1.push_front(std::move(request));
    } else if (request->track_num < dest_track){
        this->wait_queue_2.push_front(std::move(request));
    } else {
        this->active_queue->push_front(std::move(request));
    }
}

void CLOOK_IOScheduler::FetchNext() {

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
