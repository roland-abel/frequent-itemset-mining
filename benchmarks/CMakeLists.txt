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
set(DATA_DIR ${BENCHMARK_DIR}/data)
set(DATA_OUT_DIR ${CMAKE_BINARY_DIR}/benchmarks/data)

file(GLOB DATA_FILES "${DATA_DIR}/**.dat")

add_executable(
        ${BENCHMARK_NAME}
        benchmark.cpp
        ${SRC_DIR}/reader.cpp
        ${SRC_DIR}/dtypes.cpp
        ${SRC_DIR}/fp_tree.cpp
        ${SRC_DIR}/apriori.cpp
        ${SRC_DIR}/fp_growth.cpp
        ${SRC_DIR}/eclat.cpp
        ${SRC_DIR}/relim.cpp
)

find_package(benchmark CONFIG REQUIRED)

target_link_libraries(${BENCHMARK_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(${BENCHMARK_NAME} PRIVATE ${INCLUDE_DIR})

add_custom_command(
        TARGET ${BENCHMARK_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DATA_OUT_DIR}
)

foreach(DATA_FILE IN LISTS DATA_FILES)
    add_custom_command(
            TARGET ${BENCHMARK_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DATA_FILE} ${DATA_OUT_DIR}
    )
endforeach()

