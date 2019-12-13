//
// Created by CONG YU on 2019/12/10.
//

#include "IOScheduler.h"

#include <memory>
#include <iostream>
#include "IO_Schedulers/FIFO_IOScheduler.h"
#include "IO_Schedulers/SSTF_IOScheduler.h"

std::unique_ptr<IOScheduler> IOSchedulerFactory::CreateScheduler(IOSchedType io_sched_type) {
    switch (io_sched_type) {
        case IOSchedType::FIFO:  return std::make_unique<FIFO_IOScheduler>();
        case IOSchedType::SSTF:  return std::make_unique<SSTF_IOScheduler>();
//        case IOSchedType::CLOOK: sched = new CLOOK_IOScheduler(); break;
//        case IOSchedType::FLOOK: sched = new FLOOK_IOScheduler(); break;;
//        case IOSchedType::LOOK:  sched = new LOOK_IOScheduler(); break;;
        default:
            return std::make_unique<FIFO_IOScheduler>();
    }
}

void IOScheduler::SetTime(int& current_time) {
    time = current_time;
}

bool IOScheduler::IsActive() {
    return this->active_io != nullptr;
}

bool IOScheduler::IsCompleted() {
    return this->head == this->active_io->track_num;
}

void IOScheduler::ClearActive() {
    if (logger.IsLogVerbose() && this->head == this->active_io->track_num) {
        logger << this->time << ":     " << this->active_io->op_idx << " finish " << this->time - this->active_io->timestamp << "\n";
    }
    request_num++;
    active_io->finish_time = time;
    finished_requests.push_back(std::move(active_io));
}

void IOScheduler::MoveForward() {
    this->head += this->direction;
    this->total_movement++;
}

void IOScheduler::SetLogger(MyLogger &my_logger) {
    // this is copy!!!, but does not matter, bear the cost
    this->logger = my_logger;
}

void IOScheduler::Start() {
    active_io = nullptr;
    if (logger.IsLogVerbose()) {
        logger << "TRACE" << "\n";
    }
}

void IOScheduler::LogSummary() {
    for (auto const& req : finished_requests) {
        printf("%5d: %5d %5d %5d\n", req->op_idx, req->timestamp, req->start_time, req->finish_time);
        total_turnaround += req->finish_time - req->timestamp;
        total_wait_time += req->start_time - req->timestamp;
        max_wait_time = max(max_wait_time, req->start_time - req->timestamp);
    }
    printf("SUM: %d %d %.2lf %.2lf %d\n",
            this->time, this->total_movement, this->total_turnaround/this->request_num,
            this->total_wait_time/this->request_num, this->max_wait_time);
}

void IOScheduler::LogNext() {
    active_io->start_time = time;
    if (this->logger.IsLogVerbose()) {
        logger << this->time << ":     " << this->active_io->op_idx;
        logger << " issue " << this->active_io->track_num << " " << this->head << "\n";
    }
}

void IOScheduler::LogNew(const std::unique_ptr<Request>& new_request) {
    if (this->logger.IsLogVerbose()) {
        logger << new_request->timestamp << ":     " << new_request->op_idx << " add " << new_request->track_num << "\n";
    }
}

IOScheduler::~IOScheduler() = default;

