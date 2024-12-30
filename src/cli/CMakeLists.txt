# @file CMakefile
# @brief CMake configuration file for the frequent-itemset-mining CLI project.

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

find_package(CLI11 CONFIG REQUIRED)

add_executable(${CLI_EXE_NAME} main.cpp)
target_link_libraries(${CLI_EXE_NAME} PRIVATE CLI11::CLI11 ${FIM_LIB_NAME})

target_include_directories(${CLI_EXE_NAME} PRIVATE ${INCLUDE_DIR})
