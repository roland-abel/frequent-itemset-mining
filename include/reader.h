/// @file reader.h
/// @brief
///
/// @author Roland Abel
/// @date August 30, 2024
///
/// Copyright (c) 2024 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
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

#pragma once

#include <fstream>
#include <sstream>
#include <expected>
#include "dtypes.h"

namespace rules::io {

    // Reader/writer error codes
    enum class error_t {
        FILE_NOT_FOUND,
        INVALID_FORMAT,
        UNKNOWN_ERROR,
        EMPTY_ERROR
    };

    // Result type for the `read_transactions` function.
    using read_result_t = std::expected<transactions_t, error_t>;

    /// Function to read transactions from an input stream
    /// @param input The input stream to read from. This could be a file stream or any other stream.
    /// @return An expected result containing a vector of item sets if successful, or an error code if failed.
    auto read_transactions(std::istream &input) -> read_result_t;

    /// Function to read transactions from a file
    /// @param filename The name of the file to read from.
    /// @return An expected result containing a vector of item sets if successful, or an error code if failed.
    auto read_transactions(const std::string &filename) -> read_result_t;
}