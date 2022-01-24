cmake_minimum_required(VERSION 3.20)
project(code)

set(CMAKE_CXX_STANDARD 14)
set(graph.h Menu.h graph.h Menu.h auxiliar.h minHeap.h)
add_executable(code main.cpp graph.cpp Menu.cpp auxiliar.cpp)
