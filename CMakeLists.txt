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

if (ENABLE_STATIC_CHECK)
    message("CPPCheck enabled")

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
            GIT_TAG             2.3
            GIT_SHALLOW         1
            CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
            PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
#            --enable=warning,style,unusedFunction,missingInclude
            --inconclusive #проверяем маловероятные
            --std=c++11
            --verbose #более подробный анализ
            --error-exitcode=1 #роняем тесты
            --language=c++
            -I ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src/*.c
            )


endif ()


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


