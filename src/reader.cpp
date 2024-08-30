/// @file reader.cpp
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

#include "reader.h"

auto rules::io::read_transactions(std::istream &input) -> rules::io::read_result_t {
    transactions_t transactions;
    std::string line;

    if ((input.bad() || input.fail())) {
        return std::unexpected(error_t::UNKNOWN_ERROR);
    }

    while (std::getline(input, line)) {
        std::istringstream iss(line);
        itemset_t itemset;
        std::size_t number;

        while (iss >> number) {
            itemset.insert(number);
        }

        if (iss.fail() && !iss.eof()) {
            return std::unexpected(error_t::INVALID_FORMAT);
        }

        transactions.push_back(std::move(itemset));
    }

    if (transactions.empty()) {
        return std::unexpected(error_t::EMPTY_ERROR);
    }

    return transactions;
}

auto rules::io::read_transactions(const std::string &filename) -> rules::io::read_result_t {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return std::unexpected(error_t::FILE_NOT_FOUND);
    }

    return read_transactions(file);
}
