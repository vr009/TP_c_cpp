cmake_minimum_required(VERSION 3.17)
project(iz1_task C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES address.c)

add_library(address_lib STATIC ${LIB_SOURCE_FILES})
add_executable(iz1_task main.c)
target_link_libraries(iz1_task address_lib)