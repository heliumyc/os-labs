cmake_minimum_required(VERSION 3.14)
project(lab1_linker)

set(CMAKE_CXX_STANDARD 14)

add_executable(linker main.cpp tokenizer.h tokenizer.cpp parser.h parser.cpp errors.h errors.cpp)
#add_executable(test test.cpp)
add_executable(test test.cpp)