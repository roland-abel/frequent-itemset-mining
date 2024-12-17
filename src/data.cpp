/// @file data.cpp
/// @brief
///
/// @author Roland Abel
/// @date December 08, 2024
///
/// Copyright (c) 2024 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// with the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#include <istream>
#include <sstream>
#include "data.h"

namespace fim::data {
    auto compare_streams(std::istringstream &iss, const std::ostringstream &oss) -> bool {
        std::string input_line;
        std::string output_line;

        std::istringstream string_stream(oss.str());
        while (std::getline(iss, input_line)) {
            if (!std::getline(string_stream, output_line)) {
                return false;
            }
            if (input_line != output_line) {
                return false;
            }
        }
        if (std::getline(string_stream, output_line)) {
            return false;
        }
        return true;
    }
}
