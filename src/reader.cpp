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

auto fim::io::read_csv(std::istream &input, const csv_config_t &config) -> fim::io::read_result_t {
    database_t database{};
    std::string line;

    if ((input.bad() || input.fail())) {
        return std::unexpected(io_error_t::UNKNOWN_ERROR);
    }

    for (auto i = 0; i < config.skip_rows; ++i) {
        std::getline(input, line);
    }

    while (std::getline(input, line)) {
        itemset_t itemset{};

        std::stringstream line_stream(line);
        std::string number;

        try {
            while (std::getline(line_stream, number, config.separator)) {
                size_t value = std::stoull(number);
                itemset.insert(value);
            }
        } catch (const std::invalid_argument &) {
            return std::unexpected(io_error_t::INVALID_FORMAT);
        }
        catch (const std::out_of_range &) {
            return std::unexpected(io_error_t::VALUE_OUT_OF_RANGE);
        }

        if (line_stream.fail() && !line_stream.eof()) {
            return std::unexpected(io_error_t::INVALID_FORMAT);
        }

        database.push_back(std::move(itemset));
    }

    if (database.empty()) {
        return std::unexpected(io_error_t::EMPTY_ERROR);
    }

    return database;
}

auto fim::io::read_csv(const std::string_view &filename, const csv_config_t &config) -> fim::io::read_result_t {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
        return std::unexpected(io_error_t::FILE_NOT_FOUND);
    }

    return read_csv(file, config);
}
