//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_PROCESS_H
#define LAB2_SCHEDULER_PROCESS_H


class Process {
public:
    int pid;
    int arrival_time;
    int total_cpu_time;
    int cpu_burst;
    int io_burst;

    int remain_io_burst = 0;
    int remain_cpu_burst = 0; // remember, if preempted or quantum time is up, cpu burst remains for next run!!! NOT RANDOM
    int remain_cpu_time = 0;
    int finish_time = 0;
    int io_time_total = 0; // aka IT metric required in the answer
    int cpu_waiting_time = 0;
    int state_timestamp = 0;

    int static_priority;
    int dynamic_priority;

    Process(int pid, int arrival_time, int total_cpu_time, int cpu_burst, int io_burst);
};


#endif //LAB2_SCHEDULER_PROCESS_H
