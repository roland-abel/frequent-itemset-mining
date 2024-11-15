/// @file writer_tests.cpp
/// @brief Unit test for the read transactions function.
///
/// @author Roland Abel
/// @date August 31, 2024
///
/// Copyright (c) 2023 Roland Abel
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

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include "itemset.h"
#include "writer.h"

using namespace std;
using namespace fim;
using namespace fim::data;

class WriterTests : public ::testing::Test {
public:
    itemsets_t itemsets = {
            {1, 2, 3, 4, 6, 7, 8},
            {6, 7, 8},
            {1, 2, 3, 4, 6, 7, 8},
            {6},
            {3, 4, 6, 7, 8},
            {2, 3, 7, 8}
    };

    std::string get_next_line(std::istringstream &iss) {
        std::string line;
        return std::getline(iss, line) ? line : std::string{};
    }
};

TEST_F(WriterTests, WriterEmptyCsvTest) {
    std::ostringstream oss;
    const auto &result = to_csv(oss, itemsets_t{});

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::EMPTY_ERROR);
}

TEST_F(WriterTests, WriterCsvWithHeaderTest) {
    auto config = write_csv_config_t{true, ' '};

    std::ostringstream oss;
    const auto &result = to_csv(oss, itemsets, config);

    ASSERT_TRUE(result.has_value());
    std::istringstream iss(oss.str());

    EXPECT_EQ(get_next_line(iss), "itemset_length, itemset");
    EXPECT_EQ(get_next_line(iss), "7, 1 2 3 4 6 7 8");
    EXPECT_EQ(get_next_line(iss), "3, 6 7 8");
    EXPECT_EQ(get_next_line(iss), "7, 1 2 3 4 6 7 8");
    EXPECT_EQ(get_next_line(iss), "1, 6");
    EXPECT_EQ(get_next_line(iss), "5, 3 4 6 7 8");
    EXPECT_EQ(get_next_line(iss), "4, 2 3 7 8");
}
