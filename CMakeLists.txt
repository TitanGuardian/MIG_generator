cmake_minimum_required(VERSION 3.10)
project(MIG_generator)

set(CMAKE_CXX_STANDARD 17)


#set(CMAKE_CXX_FLAGS "-Ofast")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++")
#set(CMAKE_CXX_FLAGS " ")

message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP! ^_^")
    # add flags for OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP! x_x")
ENDIF()

#set(CUDA_DIR ${CUDA_PATH})
#find_package(CUDA REQUIRED)
#include_directories(${CUDA_PATH}/include)

# Compile CU files
#list( APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30; -std=c++2a")
#cuda_compile(CudaKernel src/CUDA.cu)

include_directories(./include )

add_library (MIG ./src/mig.cpp ./src/permutation.cpp ./src/SearchMutation.cpp ./src/BestSchemasDict.cpp ./src/schema_info.cpp)

add_library (GENERATOR ./src/mig_generator.cpp)
target_link_libraries (GENERATOR MIG)


add_executable(MIG_generator
        ./src/main.cpp
        )


target_link_libraries(MIG_generator
        MIG
        GENERATOR
	pthread  	      
)

add_executable(mincodebycomplexity ./src/get_mincode_complexity.cpp ) 

target_link_libraries(mincodebycomplexity
        MIG
)

add_executable(check ./src/check_correctness.cpp )

target_link_libraries(check
        MIG
        )

add_executable(shrink ./src/shrink.cpp )

target_link_libraries(shrink
        MIG
        )

