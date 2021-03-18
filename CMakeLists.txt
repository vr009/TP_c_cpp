cmake_minimum_required(VERSION 3.5)
project(iz1_task )
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")

set(LIB_SOURCE_FILES address.c)

add_library(address_lib STATIC ${LIB_SOURCE_FILES})
add_executable(iz1_task main.c)
target_link_libraries(iz1_task address_lib)

#включаем
if (ENABLE_VALGRIND_CHECK)
    message("Valgrind enabled")
    find_program(VALGRIND valgrind REQUIRED)

    if(VALGRIND)
        message("Valgrind found")
        set(VALGRIND_OPTIONS "--leak-check=full --track-origins=yes --error-exitcode=1")
    endif ()

endif ()



enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})

add_executable(tests tests.cpp)

target_link_libraries(tests ${GTEST_LIBRARIES} -lpthread)
target_link_libraries(tests address_lib)


