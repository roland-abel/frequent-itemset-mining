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
#include "itemset.h"
#include "writer.h"

using namespace fim;

namespace fim::data {
    auto to_csv(std::ostream &os, const itemsets_t &itemsets, const write_csv_config_t &config) -> write_result_t {
        using std::ranges::views::transform;
        using std::ranges::views::join_with;

        auto write_header = [&] {
            os << "itemset_length, itemset" << std::endl;
        };

        auto write_itemset = [&](const itemset_t &itemset) {
            os << itemset.size() << ", ";
            std::ranges::for_each(itemset
                                  | transform([](const item_t &item) { return std::to_string(item); })
                                  | join_with(config.separator), [&](const auto &item) { os << item; });
            os << std::endl;
        };

        if (itemsets.empty()) {
            return std::unexpected{io_error_t::EMPTY_ERROR};
        }

        if (config.with_header) {
            write_header();
        }

        for (const itemset_t &itemset: itemsets) {
            write_itemset(itemset);
        }
        return write_result_t{itemsets};
    }

    auto to_csv(
        const std::string_view &file_path,
        const itemsets_t &itemsets,
        const write_csv_config_t &config) -> write_result_t {
        std::filesystem::path path = std::filesystem::path(file_path).parent_path();

        if (!exists(path)) {
            create_directories(path);
        }

        std::ofstream ofs(file_path.data());

        if (!ofs) {
            return std::unexpected{io_error_t::UNKNOWN_ERROR};
        }

        return to_csv(ofs, itemsets, config);
    };
}
