cmake_minimum_required(VERSION 3.14)
project(linker)

set(CMAKE_CXX_STANDARD 14)

add_executable(linker main.cpp tokenizer.h tokenizer.cpp parser.h parser.cpp exception.h)
#add_executable(test test.cpp)