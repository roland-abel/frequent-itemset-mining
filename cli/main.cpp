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

#include <iostream>
#include <string>
#include "CLI/CLI.hpp"
#include "utils.h"

using namespace std;
using namespace fim;
using namespace fim::config;

namespace {
    static const std::map<std::string, algorithm_t> map_string_to_algorithm{
            {"apriori",   algorithm_t::APRIORI},
            {"fp-growth", algorithm_t::FP_GROWTH},
    };

    static const std::map<algorithm_t, std::string> map_algorithm_to_string{
            {algorithm_t::APRIORI,   "apriori"},
            {algorithm_t::FP_GROWTH, "fp-growth"}
    };

    std::ostream &operator<<(std::ostream &out, const algorithm_t algorithm) {
        auto it = map_algorithm_to_string.find(algorithm);
        return out << (it != map_algorithm_to_string.end() ? it->second : std::string{});
    };
}

void add_options(CLI::App &app, configuration_t &config) {

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
            ->option_text("(between 0 and 1)");

    app.add_option("-c, --min-confidence", config.min_confidence)
            ->description("Minimum confidence threshold")
            ->check(CLI::Range(0.0f, 1.f))
            ->option_text("(between 0 and 1)");

    app.add_option("-a, --algorithm", config.algorithm)
            ->description("Specifies which algorithm to use.")
            ->default_val(algorithm_t::APRIORI)
            ->transform(CLI::CheckedTransformer(map_string_to_algorithm, CLI::ignore_case))
            ->option_text("('apriori' or 'fp-growth')");

    app.add_option("-m, --max-length", config.max_length)
            ->description("Maximum length of the suffix to be found")
            ->option_text(" ");

    app.add_flag("-v, --verbose", config.is_verbose)
            ->description("Enables verbose mode");
}

auto main(int argc, char **argv) -> int {
    CLI::App app{"Frequent Itemset Mining"};
    app.get_formatter()->column_width(50);

    configuration_t config{};
    add_options(app, config);

    CLI11_PARSE(app, argc, argv);

    std::cout << "Input file        : " << config.input_path << std::endl;
    std::cout << "Output file       : " << config.output_path << std::endl;
    std::cout << "Minimum support   : " << config.min_support << std::endl;
    std::cout << "Minimum confidence: " << config.min_confidence << std::endl;
    std::cout << "Algorithm         : " << config.algorithm << std::endl;
    std::cout << "Verbose           : " << config.is_verbose << std::endl;

    return 0;
}
