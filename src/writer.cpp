/// @file writer.cpp
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

#include <ranges>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "itemset.h"
#include "writer.h"

using namespace std::literals;
using namespace fim;

using std::ranges::views::transform;
using std::ranges::views::join_with;

namespace fim::data {
    auto to_csv(
        std::ostream &os,
        const write_input_t &input,
        const write_csv_config_t &config) -> write_result_t {
        // set formatting style
        std::cout << std::fixed << std::setprecision(2);
        const auto &[itemsets, support_values] = input;

        auto write_header = [&] {
            const std::vector columns{"length"sv, "itemset"sv, "support"sv};
            const auto header = columns | std::views::join_with(config.separator);

            for (const auto h: header) {
                os << h;
            }

            os << std::endl;
        };

        auto write_itemset = [&](const itemset_t &itemset, const float &support) {
            constexpr auto space = " "sv;
            os << itemset.size() << config.separator;
            std::ranges::for_each(itemset
                                  | transform([](const item_t &item) { return std::to_string(item); })
                                  | join_with(space), [&](const auto &item) { os << item; });

            os << config.separator << support << std::endl;
        };

        if (itemsets.empty()) {
            return std::unexpected{io_error_t::EMPTY_ERROR};
        }

        if (config.with_header) {
            write_header();
        }

        for (const auto &&[itemset, support]: std::ranges::views::zip(itemsets, support_values)) {
            write_itemset(itemset, support);
        }
        return write_result_t{};
    }

    auto to_csv(
        const std::string_view &file_path,
        const write_input_t &input,
        const write_csv_config_t &config) -> write_result_t {
        const auto &[itemsets, support_values] = input;
        const std::filesystem::path path = std::filesystem::path(file_path).parent_path();

        if (!exists(path)) {
            create_directories(path);
        }

        std::ofstream ofs(file_path.data());

        if (!ofs) {
            return std::unexpected{io_error_t::UNKNOWN_ERROR};
        }

        return to_csv(ofs, input, config);
    }
}
