cmake_minimum_required(VERSION 3.15)
project(pthread_demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(pthread_demo main.cpp thread_fun.cpp thread_fun.h)
find_package(Threads REQUIRED)
target_link_libraries(pthread_demo Threads::Threads)