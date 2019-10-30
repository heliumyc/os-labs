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

    int remain_cpu_time;
    int state_timestamp = 0;

    int static_priority;
    int dynamic_priority;

    Process(int pid, int arrival_time, int total_cpu_time, int cpu_burst, int io_burst);
};


#endif //LAB2_SCHEDULER_PROCESS_H
