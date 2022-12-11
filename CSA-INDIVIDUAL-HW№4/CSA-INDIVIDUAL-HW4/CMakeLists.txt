cmake_minimum_required(VERSION 3.23)
project(CSA_INDIVIDUAL_HW4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(CSA_INDIVIDUAL_HW4 main.cpp)
