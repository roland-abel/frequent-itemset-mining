/// @file dtypes.h
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

#include <chrono>
#include <set>
#include <unordered_map>

namespace rules {

    // The item type.
    using item_t = unsigned long;

    // The itemset type.
    using itemset_t = std::set<item_t>;

    // Collection of itemsets.
    using itemsets_t = std::set<itemset_t>;

    // The transaction database type.
    using transactions_t = std::vector<itemset_t>;

    // The associate rule type which contains a premise
    // and a conclusion (premise -> conclusion).
    using rule_t = std::tuple<itemset_t, itemset_t>;

    // Collection of rules.
    using rules_t = std::set<rule_t>;

    // Hash code type
    using code_t = unsigned long;

    // Type for counting frequents of itemsets. The key is the hash code of an item set.
    using frequencies_t = std::unordered_map<code_t, size_t>;

    // Type for counting frequents of an item.
    using item_counts_t = std::unordered_map<item_t , size_t>;

    // Date time type
    using datetime_t = std::chrono::sys_time<std::chrono::seconds>;

    /// Hash function for an itemset.
    /// @param itemset The given itemset.
    /// @return The hash code for the given x.
    code_t hash_code(const itemset_t &itemset);

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

    namespace config {

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
    }

    namespace io {

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
            itemsets_t itemsets{};
            frequencies_t frequencies{};
            float min_support{};
            size_t num_items{};
            size_t num_transactions{};
            datetime_t creation_datetime{current_datetime()};
            algorithm_t algorithm{algorithm_t::UNKNOWN};
        };
    }
}