cmake_minimum_required(VERSION 2.8.11)
project(antwork)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")

include_directories(.)
file(GLOB src barrier.cpp  task.cpp  thread_pool.cpp  timer.cpp)
add_library(antwork STATIC ${src})

add_executable(dummy_example dummy.cpp)
target_link_libraries(dummy_example antwork pthread)
