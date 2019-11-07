# cmake_minimum_required(VERSION <specify CMake version here>)
project(MIG_generator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MIG_generator
        mig.cpp
        mig.h)
