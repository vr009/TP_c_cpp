CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(T2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -Werror -Wpedantic")

add_library(SUBSEQUENCE_STR_SIMPLE STATIC libs/simple/maxsubseq.c)
#add_library(SUBSEQUENCE_STR_PARALLEL SHARED libs/parallel/maxsubseq.c)

add_executable(T2 main.c)
target_link_libraries(T2 ${CMAKE_DL_LIBS} SUBSEQUENCE_STR_SIMPLE)