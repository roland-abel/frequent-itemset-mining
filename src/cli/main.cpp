/// @file main.cpp
/// @brief CLI main function.
///
/// @author Roland Abel
/// @date July 5, 2004
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

#include <expected>
#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include "CLI/CLI.hpp"
#include "algorithms.h"
#include "reader.h"
#include "writer.h"

using namespace std;
using namespace fim;
using namespace fim::algorithm;

struct configuration_t {
    std::string input_path;
    std::string output_path;
    float min_support;
    algorithm_t algorithm;
    bool override;
};

void add_options(CLI::App &app, configuration_t &config) {
    const std::map<std::string, algorithm_t> map_string_to_algorithm{
        {"apriori", algorithm_t::APRIORI},
        {"fp-growth", algorithm_t::FP_GROWTH},
        {"relim", algorithm_t::RELIM},
        {"eclat", algorithm_t::ECLAT}
    };

    const auto non_existent_path_validator = [&config](const std::string &path) {
        if (config.override) {
            return std::string();
        }

        const std::ifstream file(path);
        return !file.good() ? std::string() : "File already exists.";
    };

    app.add_flag("--override", config.override)
            ->description("If set, the output file will be overwritten");

    app.add_option("-i, --input", config.input_path)
            ->description("Path to the input file containing the database")
            ->required()
            ->option_text("(existing file)")
            ->check(CLI::ExistingFile);

    app.add_option("-o, --output", config.output_path)
            ->description("Path to the output file where the frequent itemsets will be saved")
            ->required()
            ->option_text("(none existing file)")
            ->check(CLI::Validator(non_existent_path_validator, "NonexistentPath"));

    app.add_option("-s, --min-support", config.min_support)
            ->description("Minimum support threshold for the frequent itemsets")
            ->check(CLI::Range(0.0f, 1.f))
            ->default_val(0.8)
            ->option_text("(between 0 and 1)");

    app.add_option("-a, --algorithm", config.algorithm)
            ->description("Specifies which algorithm to use.")
            ->default_val(algorithm_t::APRIORI)
            ->transform(CLI::CheckedTransformer(map_string_to_algorithm, CLI::ignore_case))
            ->option_text("('apriori', 'fp-growth', 'eclat', 'relim')");
}

auto main(const int argc, char **argv) -> int {
    CLI::App app{"Frequent Itemset Mining"};
    app.get_formatter()->column_width(60);

    configuration_t config{};
    add_options(app, config);

    app.callback([&] {
        constexpr auto read_config = data::read_csv_config_t{
            .skip_rows = 0,
            .separator = ','
        };

        constexpr auto write_config = data::write_csv_config_t{
            .with_header = true,
            .separator = ';'
        };

        auto read_csv = [&]() -> std::optional<tuple<database_t, size_t> > {
            const auto res = data::read_csv(config.input_path, read_config);
            if (not res) {
                return std::nullopt;
            }

            const auto &db = *res;
            return std::tuple{db, db.size()};
        };

        auto prepare_database = [&config](const auto &input) {
            const auto &[database, db_size] = input;

            const auto min_support = static_cast<size_t>(config.min_support * static_cast<float>(db_size));
            const auto &[db, item_counts] = database.transaction_reduction(min_support);
            const auto compare = item_counts.get_item_compare();

            return std::optional{std::tuple{db, item_counts, min_support, db_size}};
        };

        auto apply_algorithm = [&config](const auto &input) {
            const auto &[db, item_counts, min_support, db_size] = input;
            auto freq_items = get_algorithm(config.algorithm)({db, item_counts}, min_support)
                    .sort_each_itemset(item_counts.get_item_compare());

            return std::optional{std::tuple{db, freq_items, item_counts, db_size}};
        };

        auto count_frequencies = [&](const auto &input) {
            const auto &[db, freq_items, item_counts, db_size] = input;
            const auto &counts = itemset_counts_t::create_itemset_counts(
                db,
                freq_items,
                item_counts.get_item_compare());

            return std::optional{std::tuple{freq_items, counts, db_size}};
        };

        auto get_support_values = [&](const auto &input) {
            const auto &[freq_items, counts, db_size] = input;

            auto get_support = [&](const auto &itemset) -> float {
                return counts.get_support(itemset, db_size);
            };

            support_values_t support_values{};
            for (const auto &itemset: freq_items) {
                support_values.push_back(get_support(itemset));
            }

            return std::optional{std::tuple{freq_items, support_values}};
        };

        auto to_csv = [&](const auto &input) {
            const auto &[freq_items, support_values] = input;
            const auto write_input = data::write_input_t{
                .itemsets = freq_items,
                .support_values = support_values
            };

            return data::to_csv(config.output_path, write_input, write_config);
        };

        const auto result = read_csv()
                .and_then(prepare_database)
                .and_then(apply_algorithm)
                .and_then(count_frequencies)
                .and_then(get_support_values)
                .transform(to_csv);

        if (not result.has_value()) {
            std::cout << "An error occurred" << std::endl;
        }
    });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::cout << "Input file        : " << config.input_path << std::endl;
    std::cout << "Output file       : " << config.output_path << std::endl;
    std::cout << "Minimum support   : " << config.min_support << std::endl;
    std::cout << "Algorithm         : " << static_cast<int>(config.algorithm) << std::endl;

    return 0;
}
