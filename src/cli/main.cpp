/// @file main.cpp
/// @brief
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

#include <expected>
#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "types.h"
#include "reader.h"
#include "writer.h"

using namespace std;
using namespace fim;

void add_options(CLI::App &app, configuration_t &config) {

    const std::map<std::string, algorithm_t> map_string_to_algorithm{
            {"apriori",   algorithm_t::APRIORI},
            {"fp-growth", algorithm_t::FP_GROWTH},
            {"relim",     algorithm_t::RELIM},
            {"eclat",     algorithm_t::ECLAT}
    };

    app.add_option("-i, --input", config.input_path)
            ->description("Path to the input file containing the database")
            ->required()
            ->option_text("(existing file)")
            ->check(CLI::ExistingFile);

    app.add_option("-o, --output", config.output_path)
            ->description("Path to the output file where the frequent suffix will be saved")
            ->required()
            ->option_text("(none existing file)")
            ->check(CLI::NonexistentPath);

    app.add_option("-s, --min-support", config.min_support)
            ->description("Minimum support threshold for the frequent suffix")
            ->check(CLI::Range(0.0f, 1.f))
            ->default_val(0.8)
            ->option_text("(between 0 and 1)");

    app.add_option("-a, --algorithm", config.algorithm)
            ->description("Specifies which algorithm to use.")
            ->default_val(algorithm_t::APRIORI)
            ->transform(CLI::CheckedTransformer(map_string_to_algorithm, CLI::ignore_case))
            ->option_text("('apriori', 'fp-growth', 'eclat', 'relim')");
}

auto main(int argc, char **argv) -> int {
    CLI::App app{"Frequent Itemset Mining"};
    app.get_formatter()->column_width(50);

    configuration_t config{};
    add_options(app, config);

    app.callback([&]() {
        auto min_support_abs = [&](const database_t &db) -> size_t {
            return static_cast<size_t>(config.min_support * db.size());
        };

        auto read_config = data::read_csv_config_t{
                .skip_rows =  0,
                .separator =  ' '
        };

        auto write_config = data::write_csv_config_t{
                .with_header = true,
                .separator = ' '
        };

        auto read_csv = [&]() -> data::read_result_t {
            return data::read_csv(config.input_path, read_config);
        };

        auto to_csv = [&](const itemsets_t &itemsets) -> data::write_result_t {
            return data::to_csv(config.output_path, itemsets, write_config);
        };

        auto apply_algorithm = [&](const database_t &database) -> itemsets_t {
            const auto algorithm = get_algorithm(config.algorithm);
            auto &db = const_cast<database_t &>(database);

            return algorithm(db, min_support_abs(db));
        };

        read_csv().transform(apply_algorithm).transform(to_csv);
    });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::cout << "Input file        : " << config.input_path << std::endl;
    std::cout << "Output file       : " << config.output_path << std::endl;
    std::cout << "Minimum support   : " << config.min_support << std::endl;
    std::cout << "Algorithm         : " << (int) config.algorithm << std::endl;

    return 0;
}
