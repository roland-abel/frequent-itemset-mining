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
project(association-rules
        VERSION 1.0.0
        DESCRIPTION "Implementation of the Apriori and FP-Growth algorithms"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(EXECUTABLE_NAME rule)
set(BENCHMARK_NAME benchmark)
set(PROJECT_NAME association-rules)
set(RULES_LIB rules)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(DATA_FILES ${CMAKE_SOURCE_DIR}/data/retail.dat)

set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(
        ${EXECUTABLE_NAME}
        main.cpp
        src/apriori.cpp
        src/fp_growth.cpp
        src/reader.cpp
)

add_executable(
        ${BENCHMARK_NAME}
        benchmark.cpp
        src/apriori.cpp
        src/fp_growth.cpp
        src/reader.cpp
)

find_package(CLI11 CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

target_link_libraries(${BENCHMARK_NAME} PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)

target_include_directories(${BENCHMARK_NAME}
        PRIVATE ${INCLUDE_DIR}
)

add_custom_command(
        TARGET ${BENCHMARK_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DATA_FILES} ${CMAKE_BINARY_DIR}
        COMMENT "Kopiere die Resource-Datei in das Output-Verzeichnis"
)


target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        CLI11::CLI11
)

target_include_directories(${EXECUTABLE_NAME}
        PRIVATE ${INCLUDE_DIR}
)


add_subdirectory(test)
add_subdirectory(src)
