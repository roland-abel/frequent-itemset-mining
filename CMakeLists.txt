# @file CMakefile
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

cmake_minimum_required(VERSION 3.22.1)
project(frequent-itemset-mining
        VERSION 1.0.0
        DESCRIPTION "Implementation of the frequent itemset mining algorithms"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(PROJECT_NAME frequent-itemset-mining)
set(FIM_LIB fimlib)
set(CLI_EXE_NAME fim)
set(BENCHMARK_NAME benchmark-tests)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(
        ${CLI_EXE_NAME}
        main.cpp
        ${SRC_DIR}/dtypes.cpp
        ${SRC_DIR}/fp_tree.cpp
        ${SRC_DIR}/apriori.cpp
        ${SRC_DIR}/fp_growth.cpp
        ${SRC_DIR}/reader.cpp
)

find_package(CLI11 CONFIG REQUIRED)

target_link_libraries(${CLI_EXE_NAME} PRIVATE CLI11::CLI11)
target_include_directories(${CLI_EXE_NAME} PRIVATE ${INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
