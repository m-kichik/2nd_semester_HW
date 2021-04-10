cmake_minimum_required(VERSION 3.17)
project(chrono)

set (CMAKE_C_COMPILER_WORKS 1)
set (CMAKE_CXX_COMPILER_WORKS 1)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-Wdeprecated-declarations -pthread")
set (CMAKE_CXX_FLAGS "-Wall")
#set(Boost_USE_STATIC_LIBS ON)
#set(SFML_DIR C:/SFML-2.5.1)
#set(SFML_ROOT ${SFML_DIR})
#set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
#find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

find_package(Boost 1.71.0 REQUIRED COMPONENTS locale)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(hw9_chat HW_9/chat.cpp)
add_executable(hw9_protector HW_9/protector.cpp)

add_executable(server HW_10/SERVER.cpp)
add_executable(client HW_10/CLIENT.cpp)

add_executable(TASK1 HW_8/1st_task_CALCULATE_PI_UPDATED.cpp)
add_executable(TASK2 "HW_8/2nd task _ DNA SEARCH/DNA_SEARCH.cpp")
add_executable(TASK3 HW_8/3rd_task_SAFETY_PRIORITY_QUEUE.cpp)
add_executable(TASK4 "HW_8/4th task _ COMPETITION/4th_task_COMPETITION.cpp")

add_executable(test Test/test.cpp)

target_link_libraries(server wsock32 ws2_32)
target_link_libraries(client wsock32 ws2_32)
#target_link_libraries(chrono Boost::locale)