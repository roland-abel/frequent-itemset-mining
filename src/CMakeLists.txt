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

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

find_package(nlohmann_json CONFIG REQUIRED)

add_library(${FIM_LIB_NAME}
        itemset.cpp
        cli/utils.cpp
        hash_tree.cpp
        reader.cpp
        writer.cpp
        apriori.cpp
        apriori_hash.cpp
        fp_tree.cpp
        fp_growth.cpp
        eclat.cpp
        relim.cpp
)

target_link_libraries(${FIM_LIB_NAME} PRIVATE nlohmann_json::nlohmann_json)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

target_include_directories(${FIM_LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${INCLUDE_DIR})
