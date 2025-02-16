/// @file reader.h
/// @brief Helper functions for reading transaction from CSV files.
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

#pragma once

#include <expected>
#include "itemset.h"
#include "database.h"
#include "data.h"

namespace fim::data {
    /// Configuration for reading of csv data
    struct read_csv_config_t {
        size_t skip_rows = 0;
        char separator = ' ';
    };

    /// Result type
    using read_result_t = std::expected<database_t, io_error_t>;

    /// Reads the transactions from the input stream as CSV.
    /// @param is The input stream to read from.
    /// @param config The configuration structure to specify the behavior of the reader.
    /// @return The transactions if successful, or an error code if failed.
    auto read_csv(std::istream &is, const read_csv_config_t &config = read_csv_config_t{}) -> read_result_t;

    /// Reads transactions from the given file.
    /// @param file_path The path of the file to read from.
    /// @param config The configuration structure to specify the behavior of the reader.
    /// @return The transactions if successful, or an error code if failed.
    auto read_csv(
        const std::string_view &file_path,
        const read_csv_config_t &config = read_csv_config_t{}) -> read_result_t;
}
