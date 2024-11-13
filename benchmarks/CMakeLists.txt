# @file CMakefile
#
# @author Roland Abel
# @date September 21, 2024
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

set(BENCHMARK_DIR ${CMAKE_SOURCE_DIR}/benchmarks)
set(BENCHMARK_DATA_DIR ${BENCHMARK_DIR}/data)
set(BENCHMARK_DATA_OUT_DIR ${CMAKE_BINARY_DIR}/benchmarks/data)

file(GLOB BENCHMARK_DATA_FILES "${BENCHMARK_DATA_DIR}/**.dat")
file(GLOB BENCHMARK_TESTS "**_benchmark.cpp")

add_executable(${BENCHMARK_NAME}
        ${BENCHMARK_TESTS}
        benchmark_main.cpp)

find_package(benchmark CONFIG REQUIRED)

target_include_directories(${BENCHMARK_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${BENCHMARK_NAME}
        PRIVATE benchmark::benchmark benchmark::benchmark_main
        ${FIM_LIB_NAME})

message("Create folder ${BENCHMARK_DATA_OUT_DIR}")
file(MAKE_DIRECTORY ${BENCHMARK_DATA_OUT_DIR})

foreach(FILE IN LISTS BENCHMARK_DATA_FILES)
    message("Copy data file ${FILE}")
    file(COPY ${FILE} DESTINATION ${BENCHMARK_DATA_OUT_DIR})
endforeach()
