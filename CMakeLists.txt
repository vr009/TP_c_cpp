cmake_minimum_required(VERSION 3.5)
project(iz1_task )
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(LIB_SOURCE_FILES address.c)

add_library(address_lib STATIC ${LIB_SOURCE_FILES})
add_executable(iz1_task main.c)
target_link_libraries(iz1_task address_lib)


enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(tests tests.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests address_lib)



