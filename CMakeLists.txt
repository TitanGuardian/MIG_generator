cmake_minimum_required(VERSION 3.10)
project(MIG_generator)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "-Ofast")
#set(CMAKE_CXX_FLAGS "-L \"C:/Program Files (x86)/mingw64/bin\" -ltbb")
#set(CMAKE_CXX_FLAGS " ")


#set(CUDA_DIR ${CUDA_PATH})
#find_package(CUDA REQUIRED)
#include_directories(${CUDA_PATH}/include)

# Compile CU files
#list( APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30; -std=c++2a")
#cuda_compile(CudaKernel src/CUDA.cu)

include_directories(./include )



add_executable(MIG_generator
        ./src/main.cpp
        ./src/mig.cpp
        ./src/permutation.cpp
        ./src/SearchMutation.cpp
        ./src/mig_generator.cpp
        ./src/BestSchemasDict.cpp
        )

target_link_libraries(MIG_generator pthread)