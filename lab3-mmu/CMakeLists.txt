cmake_minimum_required(VERSION 3.15)
project(lab3_mmu)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3_mmu main.cpp Pager.cpp Pager.h DataTypes.h Pagers/FIFO_Pager.cpp Pagers/FIFO_Pager.h MyReader.cpp MyReader.h Process.cpp Process.h MyLogger.cpp MyLogger.h Pagers/Random_Pager.cpp Pagers/Random_Pager.h MyRandomGenerator.cpp MyRandomGenerator.h Pagers/Clock_Pager.cpp Pagers/Clock_Pager.h Pagers/ESC_NRU_Pager.cpp Pagers/ESC_NRU_Pager.h Pagers/Aging_Pager.cpp Pagers/Aging_Pager.h Pagers/WorkingSet_Pager.cpp Pagers/WorkingSet_Pager.h)