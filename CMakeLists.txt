cmake_minimum_required(VERSION 3.17)
project(chrono)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost)
#INCLUDE_DIRECTORIES( ${Boost_INCLUDEDIR} )

add_executable(chrono 1st_lesson_task_4_TIMER.cpp)