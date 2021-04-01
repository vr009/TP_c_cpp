CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(T2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -Werror -Wpedantic")

add_library(FILE_WORKER STATIC libs/file_worker/file_worker.c)
add_library(SUBSEQUENCE_STR_SIMPLE STATIC libs/simple/maxsubseq.c)
add_library(SUBSEQUENCE_STR_PARALLEL SHARED libs/parallel/maxsubseq.c)


file(COPY test/autotest.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY test/autotest.txt
        DESTINATION ../libs/file_worker)
file(COPY test/autotest.txt
        DESTINATION ../libs/parallel)
file(COPY test/autotest.txt
        DESTINATION ../libs/simple)

file(COPY test/aaa.txt
        DESTINATION ../libs/file_worker)
file(COPY test/aaa.txt
        DESTINATION ../libs/parallel)
file(COPY test/aaa.txt
        DESTINATION ../libs/simple)

file(COPY test/simple_test.txt
        DESTINATION ../libs/file_worker)
file(COPY test/simple_test.txt
        DESTINATION ../libs/parallel)
file(COPY test/simple_test.txt
        DESTINATION ../libs/simple)



add_executable(T2 main.c)
target_link_libraries(T2 ${CMAKE_DL_LIBS} SUBSEQUENCE_STR_SIMPLE )
target_link_libraries(T2 ${CMAKE_DL_LIBS} FILE_WORKER )



target_link_libraries(T2 ${CMAKE_DL_LIBS} SUBSEQUENCE_STR_PARALLEL)


enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(tests_simple libs/simple/test_simple.cpp )
add_executable(tests_parallel libs/parallel/test_parallel.cpp )
add_executable(tests_file_worker libs/file_worker/test_file_worker.cpp )


target_link_libraries(tests_simple ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests_simple  SUBSEQUENCE_STR_SIMPLE)
target_link_libraries(tests_simple FILE_WORKER)

target_link_libraries(tests_parallel ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests_parallel SUBSEQUENCE_STR_PARALLEL)
target_link_libraries(tests_parallel FILE_WORKER)

target_link_libraries(tests_file_worker ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests_file_worker FILE_WORKER)
