cmake_minimum_required(VERSION 4.1)
project(MLFQ)

set(CMAKE_CXX_STANDARD 17)

add_executable(MLFQ main.cpp
        MLFQ.cpp
        MLFQ.hpp
        Queue.cpp
        Queue.hpp
        Process.cpp
        Process.hpp
        IOQueue.cpp
        IOQueue.hpp)
