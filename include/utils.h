/// @file utils.h
/// @brief
///
/// @author Roland Abel
/// @date July 07, 2024
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

#include <set>
#include <unordered_map>
#include <chrono>
#include "itemset.h"

namespace fim {

    // Date time type
    using datetime_t = std::chrono::sys_time<std::chrono::seconds>;

    ///
    /// @param year
    /// @param month
    /// @param day
    /// @param hours
    /// @param minutes
    /// @param seconds
    /// @return
    auto to_datetime(
            std::chrono::year year,
            std::chrono::month month,
            std::chrono::day day,
            std::chrono::hours hours,
            std::chrono::minutes minutes,
            std::chrono::seconds seconds) -> datetime_t;

    ///
    /// @param datetime
    /// @return
    auto iso8601_datetime(const datetime_t &datetime) -> std::string;

    ///
    /// @return
    auto current_datetime() -> datetime_t;

    ///
    enum class algorithm_t : int {
        UNKNOWN,
        APRIORI,
        FP_GROWTH,
    };

    ///
    /// @param algorithm
    /// @return
    auto to_string(algorithm_t algorithm) -> std::string;

    ///
    /// @param str
    /// @return
    auto to_algorithm(std::string_view str) -> algorithm_t;

    ///
    enum class input_format_t {
        CVS,
        JSON
    };

    ///
    struct configuration_t {
        std::string input_path;
        std::string output_path;
        float min_support;
        float min_confidence;
        algorithm_t algorithm;
        uint8_t max_length;
        bool is_verbose;
        // min_lift=3,
        // min_length=2,
    };

    // Reader/writer error codes
    enum class io_error_t {
        FILE_NOT_FOUND,
        INVALID_FORMAT,
        VALUE_OUT_OF_RANGE,
        UNKNOWN_ERROR,
        EMPTY_ERROR
    };

    ///
    struct frequency_output_t {
        itemset::itemsets_t itemsets{};
        itemset::itemset_count_t frequencies{};
        float min_support{};
        size_t num_items{};
        size_t num_transactions{};
        datetime_t creation_datetime{current_datetime()};
        algorithm_t algorithm{algorithm_t::UNKNOWN};
    };
}