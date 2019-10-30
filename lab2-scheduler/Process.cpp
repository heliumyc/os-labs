//
// Created by CONG YU on 2019/10/28.
//

#include "Process.h"

Process::Process(int arrival_time, int total_cpu_time, int cpu_burst, int io_burst) : arrival_time(arrival_time),
                                                                                 total_cpu_time(total_cpu_time),
                                                                                 cpu_burst(cpu_burst), io_burst(io_burst) {
    this->remain_cpu_time = total_cpu_time;
    this->static_priority = -1; // dummy init
    this->dynamic_priority = -1; // dummy init
}
