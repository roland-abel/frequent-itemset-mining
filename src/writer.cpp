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

#include <nlohmann/json.hpp>
#include "rules.h"
#include "writer.h"

using namespace nlohmann;
using namespace fim::itemset;

namespace fim::data {

//    namespace {
//        constexpr std::string_view APRIORI = "Apriori";
//        constexpr std::string_view FP_GROWTH = "FP-Growth";
//        constexpr std::string_view UNKNOWN = "Unknown";
//    }
//
//    constexpr std::array<std::pair<algorithm_t, std::string_view>, 2> algorithm_map = {
//            {
//                    {algorithm_t::APRIORI, APRIORI},
//                    {algorithm_t::FP_GROWTH, FP_GROWTH},
//            }};
//
//    constexpr std::string_view to_string(const algorithm_t algorithm) {
//        for (const auto &pair: algorithm_map) {
//            if (pair.first == algorithm) return pair.second;
//        }
//        return UNKNOWN;
//    }
//
//    constexpr algorithm_t to_algorithm(std::string_view str) {
//        for (const auto &pair: algorithm_map) {
//            if (pair.second == str) return pair.first;
//        }
//        return algorithm_t::UNKNOWN;
//    }

    auto operator<<(std::ostream &os, const frequency_output_t &result) -> std::ostream & {
        json document_json{};
        document_json["metadata"] = {
                {"min_support",      result.min_support},
                {"num_transactions", result.num_transactions},
                {"num_items",        result.num_items},
                {"algorithm",        to_string(result.algorithm)},
                {"creation_date",    iso8601_datetime(result.creation_datetime)},
        };

        for (const auto &itemset: result.itemsets) {
            const auto frequency = result.frequencies.at(itemset);
            document_json["frequent_itemsets"].push_back(json{
                    {"itemset",   itemset},
                    {"frequency", frequency},
            });
        }
        return os << document_json;
    }

    std::istream &operator>>(std::istream &is, frequency_output_t &result) {
        json j;
        is >> j;

        result.frequencies = {};
        result.itemsets = {};

        result.min_support = j["metadata"]["min_support"];
        result.num_transactions = j["metadata"]["num_transactions"];
        result.num_items = j["metadata"]["num_items"];
        result.algorithm = to_algorithm(j["metadata"]["algorithm"]);

        auto frequent_itemsets = j["frequent_itemsets"];

        // TODO

//        for (const auto x: frequent_itemsets) {
//            const auto itemset = x["itemset"].template get<itemset_t>();
//            const auto frequency = x["frequency"].template get<size_t>();
//            const auto hash = hash_code(itemset);
//
//            result.frequencies[hash] = frequency;
//            result.itemsets.insert(itemset);
//        }
        return is;
    }

    auto to_json(const frequency_output_t &output) -> std::string {
        auto get_support = [&](const itemset_t &x) {
            return static_cast<float>(output.frequencies.at(x) / output.num_transactions);
        };

        json document_json{};
        document_json["metadata"] = {
                {"min_support",      std::format("{:.2f}", output.min_support)},
                {"num_transactions", output.num_transactions},
                {"num_items",        output.num_items},
                {"algorithm",        to_string(output.algorithm)},
                {"creation_date",    iso8601_datetime(output.creation_datetime)},
        };

        for (const auto &itemset: output.itemsets) {
            const auto support = get_support(itemset);
            document_json["frequent_itemsets"].push_back(nlohmann::json{
                    {"itemset", itemset},
                    {"support", std::format("{:.2f}", support)},
            });
        }
        return document_json.dump(4);
    }
}