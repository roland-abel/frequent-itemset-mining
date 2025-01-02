# @file CMakefile
# @brief CMake configuration file for the frequent-itemset-mining unit tests.
#
# @author Roland Abel
# @date July 02, 2024
#
# Copyright (c) 2024 Roland Abel
#
# This software is released under the MIT License.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

set(UNIT_TEST_NAME unit-tests)

find_package(GTest CONFIG REQUIRED)
enable_testing()

set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(TEST_DATA_DIR ${TEST_DIR}/data)
set(TEST_DATA_OUT_DIR ${CMAKE_BINARY_DIR}/tests/data)

file(GLOB TEST_DATA_FILES "${TEST_DATA_DIR}/**.csv")
file(GLOB UNIT_TESTS "**_tests.cpp")

add_executable(${UNIT_TEST_NAME}
        ${UNIT_TESTS}
        test_main.cpp)

target_include_directories(${UNIT_TEST_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${UNIT_TEST_NAME}
        ${FIM_LIB_NAME}
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main)

message("Create folder ${TEST_DATA_OUT_DIR}")
file(MAKE_DIRECTORY ${TEST_DATA_OUT_DIR})

foreach(FILE ${TEST_DATA_FILES})
    message("Copy data file ${FILE}")
    file(COPY ${FILE} DESTINATION ${TEST_DATA_OUT_DIR})
endforeach()
