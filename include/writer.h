/// @file writer.h
/// @brief
///
/// @author Roland Abel
/// @date August 31, 2024
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
#include <item_counts.h>
#include "data.h"

namespace fim::data {
    /// Configuration for writing of csv data
    struct write_csv_config_t {
        bool with_header{true};
        char separator{' '};
    };

    struct write_input_t {
        itemsets_t itemsets{};
        support_values_t support_values{};
    };

    /// Result type
    using write_result_t = std::expected<void, io_error_t>;

    /// @brief Writes a collection of itemsets to a CSV format.
    /// @param os The output stream where the CSV data will be written.
    /// @param input A struct of the frequent itemsets and their support to be written to the CSV.
    /// @param config Configuration settings that control the CSV writing behavior (optional).
    /// @return The result of the writing operation, indicating success or failure.
    auto to_csv(
        std::ostream &os,
        const write_input_t &input,
        const write_csv_config_t &config = write_csv_config_t{}) -> write_result_t;

    /// @brief Writes a collection of itemsets to a CSV file.
    /// @param file_path The path to the file where the CSV data will be written.
    /// @param input A struct of the frequent itemsets and their support to be written to the CSV.
    /// @param config Configuration settings that control the CSV writing behavior (optional).
    /// @return The result of the writing operation, indicating success or failure.
    auto to_csv(
        const std::string_view &file_path,
        const write_input_t &input,
        const write_csv_config_t &config = write_csv_config_t{}) -> write_result_t;
}
