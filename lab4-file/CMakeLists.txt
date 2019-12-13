cmake_minimum_required(VERSION 3.15)
project(lab4_file)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab4_file main.cpp MyReader.cpp MyReader.h IOScheduler.cpp IOScheduler.h IO_Schedulers/FIFO_IOScheduler.cpp IO_Schedulers/FIFO_IOScheduler.h CommonTypes.h MyLogger.cpp MyLogger.h IO_Schedulers/SSTF_IOScheduler.cpp IO_Schedulers/SSTF_IOScheduler.h IO_Schedulers/LOOK_IOScheduler.cpp IO_Schedulers/LOOK_IOScheduler.h)