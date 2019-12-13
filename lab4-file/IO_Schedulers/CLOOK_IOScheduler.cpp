//
// Created by CONG YU on 2019/12/13.
//

#include "CLOOK_IOScheduler.h"

bool CLOOK_IOScheduler::IsPending() {
    return false;
}

void CLOOK_IOScheduler::AddNewIORequest(std::unique_ptr<Request> &&request) {

}

void CLOOK_IOScheduler::FetchNext() {

}
