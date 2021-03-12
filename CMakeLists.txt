cmake_minimum_required(VERSION 3.17)
project(chrono)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-Wdeprecated-declarations")
set (CMAKE_CXX_FLAGS "-Wall")
#set(Boost_USE_STATIC_LIBS ON)
#set (Boost_INCLUDE_DIR C:/boost_1_75_0/)
#set (Boost_LIBRARY_DIR C:/boost_1_75_0/boost/lib/)
#set(SFML_DIR C:/SFML-2.5.1)
#set(SFML_ROOT ${SFML_DIR})
#set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
#find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,1000000")


#find_package(Boost 1.71.0 REQUIRED COMPONENTS locale)
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(chrono "HW_6/1st task _ DELETING COMMENTS/DELETING_COMMENTS.cpp")
# ClassWork/11_03_2021/11_03_2021.cpp
# "HW_6/2nd task _ STRING SEARCHING/task2_STRING_SEARCHING.cpp"

#target_link_libraries(chrono Boost::locale)