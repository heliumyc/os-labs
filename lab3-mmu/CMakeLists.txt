cmake_minimum_required(VERSION 3.15)
project(lab3_mmu)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3_mmu main.cpp Pager.cpp Pager.h DataTypes.h Pagers/FIFO_Pager.cpp Pagers/FIFO_Pager.h MyReader.cpp MyReader.h Process.cpp Process.h)