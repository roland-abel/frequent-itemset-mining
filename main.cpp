/// @file main.cpp
/// @brief
///
/// @author Roland Abel
/// @date March  8, 2004
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

#include "CLI/CLI.hpp"
#include <iostream>

auto main(int argc, char **argv) -> int {
    CLI::App app{"App description"};

    float min_supp{};
    app.add_option("-s, --min-supp", min_supp, "Minimum support threshold for the frequent itemsets.");

    float min_conf{};
    app.add_option("-c, --min-conf", min_conf, "Minimum confidence threshold.");

    std::string input_file{};
    app.add_option("-i, --input_file", input_file, "Transaction database input file");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Minimum support   : " << min_supp << std::endl;
    std::cout << "Minimum confidence: " << min_conf << std::endl;

    return 0;
}
