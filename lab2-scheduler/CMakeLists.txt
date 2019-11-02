cmake_minimum_required(VERSION 3.15)
project(lab2_scheduler)

set(CMAKE_CXX_STANDARD 14)

#add_executable(lab2_scheduler
#    main.cpp Event.cpp Event.h Process.h Scheduler.h Scheduler.cpp Schedulers/FCFS_Scheduler.cpp Schedulers/FCFS_Scheduler.h Schedulers/LCFS_Scheduler.cpp Schedulers/LCFS_Scheduler.h Schedulers/SRTF_Scheduler.cpp Schedulers/SRTF_Scheduler.h Schedulers/RoundRobin_Scheduler.cpp Schedulers/RoundRobin_Scheduler.h Schedulers/Priority_Scheduler.cpp Schedulers/Priority_Scheduler.h Schedulers/Preemptive_Priority_Scheduler.cpp Schedulers/Preemptive_Priority_Scheduler.h Process.cpp)
add_executable(sched
        main.cpp Event.cpp Event.h Process.h Scheduler.h Scheduler.cpp Schedulers/FCFS_Scheduler.cpp Schedulers/FCFS_Scheduler.h Schedulers/LCFS_Scheduler.cpp Schedulers/LCFS_Scheduler.h Schedulers/SRTF_Scheduler.cpp Schedulers/SRTF_Scheduler.h Schedulers/RoundRobin_Scheduler.cpp Schedulers/RoundRobin_Scheduler.h Schedulers/Priority_Scheduler.cpp Schedulers/Priority_Scheduler.h Schedulers/Preemptive_Priority_Scheduler.cpp Schedulers/Preemptive_Priority_Scheduler.h Process.cpp)
