//
// Created by CONG YU on 2019/12/10.
//

#include <iostream>
#include "FIFO_IOScheduler.h"

bool FIFO_IOScheduler::IsPending() {
    return !this->wait_queue.empty();
}

void FIFO_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    this->wait_queue.push(move(request));
}

void FIFO_IOScheduler::FetchNext() {
    this->active_io = move(wait_queue.front());
    this->wait_queue.pop();
    this->direction = this->active_io->track_num > this->head? 1 : -1;
}
