//
// Created by CONG YU on 2019/12/10.
//

#include <iostream>
#include "FIFO_IOScheduler.h"

bool FIFO_IOScheduler::IsPending() {
    return !wait_queue.empty();
}

void FIFO_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {
    wait_queue.push(move(request));
}

void FIFO_IOScheduler::FetchNextAndStartNewIO() {
    active_io = move(wait_queue.front());
    wait_queue.pop();
}

void FIFO_IOScheduler::MoveForward() {
    head++;
}
