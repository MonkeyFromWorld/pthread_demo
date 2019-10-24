cmake_minimum_required(VERSION 3.15)
project(pthread_demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(pthread_demo thread_fun.h thread_fun.cpp main.cpp)
find_package(Threads REQUIRED)
target_link_libraries(pthread_demo Threads::Threads)