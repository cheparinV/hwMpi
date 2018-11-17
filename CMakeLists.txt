cmake_minimum_required(VERSION 3.12)
project(untitled0)
project(untitled1)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER  /usr/bin/mpic++.openmpi)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/infotech/CLionProjects/untitled1/runtime)

add_executable(untitled0 firstTask.cpp thirdTask.cpp)
add_executable(untitled1 secondTask.cpp)
add_executable(untitled2 thirdTask.cpp)