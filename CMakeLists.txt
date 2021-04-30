cmake_minimum_required(VERSION 3.17)
project(chrono)

set (CMAKE_C_COMPILER_WORKS 1)
set (CMAKE_CXX_COMPILER_WORKS 1)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-Wdeprecated-declarations -pthread")
set (CMAKE_CXX_FLAGS "-Wall")
#set(Boost_USE_STATIC_LIBS ON)
set(SFML_DIR C:/dev/SFML-2.5.1)
set(SFML_ROOT ${SFML_DIR})
set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)
include_directories(C:/dev/SFML-2.5.1/include)
#include_directories(C:/SFML-2.5.1/include)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

find_package(Boost 1.71.0 REQUIRED COMPONENTS locale)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(gas "HW_12/3rd task _ GASE/Molecule.hpp" "HW_12/3rd task _ GASE/Main.cpp"
        "HW_12/3rd task _ GASE/Molecule.cpp"
        "HW_12/3rd task _ GASE/Functions.cpp" "HW_12/3rd task _ GASE/Functions.h"
        "HW_12/3rd task _ GASE/Visualizer.cpp" "HW_12/3rd task _ GASE/Visualizer.h")

add_executable(fractal "ClassWork/2021_04_29/mandelbrot/main.cpp"
        "ClassWork/2021_04_29/mandelbrot/calculator.cpp" "ClassWork/2021_04_29/mandelbrot/calculator.hpp"
        "ClassWork/2021_04_29/mandelbrot/view.hpp"
        "ClassWork/2021_04_29/mandelbrot/visualizer.cpp" "ClassWork/2021_04_29/mandelbrot/visualizer.hpp")

add_executable(fractal2 "HW_12/1st task _ JULIA/main.cpp"
        "HW_12/1st task _ JULIA/calculator.cpp" "HW_12/1st task _ JULIA/calculator.hpp"
        "HW_12/1st task _ JULIA/view.hpp"
        "HW_12/1st task _ JULIA/visualizer.cpp" "HW_12/1st task _ JULIA/visualizer.hpp")

add_executable(particles "ClassWork/2021_04_29/particles/main.cpp"
        "ClassWork/2021_04_29/particles/link.cpp" "ClassWork/2021_04_29/particles/link.hpp"
        "ClassWork/2021_04_29/particles/Molecula.hpp"
        "ClassWork/2021_04_29/particles/particle.cpp" "ClassWork/2021_04_29/particles/particle.cpp"
        "ClassWork/2021_04_29/particles/pressure.cpp" "ClassWork/2021_04_29/particles/pressure.hpp"
        "ClassWork/2021_04_29/particles/system.cpp" "ClassWork/2021_04_29/particles/system.hpp")

add_executable(TASK1 HW_8/1st_task_CALCULATE_PI_UPDATED.cpp)
add_executable(TASK2 "HW_8/2nd task _ DNA SEARCH/DNA_SEARCH.cpp")
add_executable(TASK3 HW_8/3rd_task_SAFETY_PRIORITY_QUEUE.cpp)
add_executable(TASK4 "HW_8/4th task _ COMPETITION/4th_task_COMPETITION.cpp")

add_executable(test Test/test.cpp Tinkoff_contest/t3.cpp)

#target_link_libraries(game ${SFML_LIBRARIES})
target_link_libraries(gas sfml-graphics sfml-system sfml-window)
target_link_libraries(fractal sfml-graphics sfml-system sfml-window)
target_link_libraries(fractal2 sfml-graphics sfml-system sfml-window)
target_link_libraries(particles sfml-graphics sfml-system sfml-window)
target_link_libraries(gas wsock32 ws2_32)

#target_link_libraries(chrono Boost::locale)