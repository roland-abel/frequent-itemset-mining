/// @file dtypes.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 6, 2024
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

#include <array>
#include <map>
#include "utils.h"

namespace fim {

    namespace {
        constexpr std::string_view APRIORI = "apriori";
        constexpr std::string_view FP_GROWTH = "fp-growth";
        constexpr std::string_view UNKNOWN = "unknown";
    }

    const auto map_string_to_algorithm = std::map<std::string_view, algorithm_t>{
            {APRIORI,   algorithm_t::APRIORI},
            {FP_GROWTH, algorithm_t::FP_GROWTH}
    };

    const auto map_algorithm_to_string = std::map<algorithm_t, std::string_view>{
            {algorithm_t::APRIORI,   APRIORI},
            {algorithm_t::FP_GROWTH, FP_GROWTH},
            {algorithm_t::UNKNOWN,   UNKNOWN}
    };

    auto to_string(algorithm_t algorithm) -> std::string {
        return std::string(map_algorithm_to_string.at(algorithm));
    }

    auto to_algorithm(std::string_view str) -> algorithm_t {
        const auto it = map_string_to_algorithm.find(str);
        return it != map_string_to_algorithm.end() ? it->second : algorithm_t::UNKNOWN;
    }

    code_t hash_code(const itemset_t &x) {
        code_t hash = 0;
        for (const auto &item: x) {
            hash ^= std::hash<item_t>()(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }

    auto to_datetime(
            std::chrono::year year,
            std::chrono::month month,
            std::chrono::day day,
            std::chrono::hours hours,
            std::chrono::minutes minutes,
            std::chrono::seconds seconds) -> datetime_t {

        return std::chrono::sys_days{std::chrono::year_month_day{year, month, day}} + hours + minutes + seconds;
    }

    auto iso8601_datetime(const datetime_t &datetime) -> std::string {
        return std::format("{:%FT%TZ}", std::chrono::floor<std::chrono::seconds>(datetime));
    }

    auto current_datetime() -> datetime_t {
        const auto &now = std::chrono::system_clock::now();
        return std::chrono::time_point_cast<std::chrono::seconds>(now);
    }
}