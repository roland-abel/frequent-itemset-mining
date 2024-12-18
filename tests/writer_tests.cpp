/// @file writer_tests.cpp
/// @brief Unit test for the writer's function.
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

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "itemset.h"
#include "writer.h"

using namespace std;
using namespace fim;
using namespace fim::data;

class WriterTests : public testing::Test {
public:
    static write_input_t get_data() {
        const itemsets_t itemsets = {
            {1, 2, 3, 4, 6, 7, 8},
            {6, 7, 8},
            {1, 2, 3, 4, 6, 7, 8},
            {6},
            {3, 4, 6, 7, 8},
            {2, 3, 7, 8}
        };
        const support_values_t support_values = {
            1.8, 2.7, 3.6, 4.5, 6.4, 7.3
        };
        return {itemsets, support_values};
    }

    static std::string get_next_line(std::istringstream &iss) {
        std::string line;
        return std::getline(iss, line) ? line : std::string{};
    }
};

TEST_F(WriterTests, WriterEmptyCsvTest) {
    std::ostringstream oss;
    const auto &result = to_csv(oss, write_input_t{});

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::EMPTY_ERROR);
}

TEST_F(WriterTests, WriterCsvWithCommaSeparatorTest) {
    std::ostringstream oss;
    const auto &result = to_csv(oss, get_data(), write_csv_config_t{true, ','});

    ASSERT_TRUE(result.has_value());
    std::istringstream iss(oss.str());

    EXPECT_EQ(get_next_line(iss), "length,itemset,support");
}

TEST_F(WriterTests, WriterCsvWithCommaSemicolonTest) {
    std::ostringstream oss;
    const auto &result = to_csv(oss, get_data(), write_csv_config_t{true, ';'});

    ASSERT_TRUE(result.has_value());
    std::istringstream iss(oss.str());

    EXPECT_EQ(get_next_line(iss), "length;itemset;support");
}

TEST_F(WriterTests, WriterCsvWithHeaderTest) {
    auto config = write_csv_config_t{true, ','};

    std::ostringstream oss;
    const auto &result = to_csv(oss, get_data(), config);

    ASSERT_TRUE(result.has_value());
    std::istringstream iss(oss.str());

    EXPECT_EQ(get_next_line(iss), "length,itemset,support");
    EXPECT_EQ(get_next_line(iss), "7,1 2 3 4 6 7 8,1.8");
    EXPECT_EQ(get_next_line(iss), "3,6 7 8,2.7");
    EXPECT_EQ(get_next_line(iss), "7,1 2 3 4 6 7 8,3.6");
    EXPECT_EQ(get_next_line(iss), "1,6,4.5");
    EXPECT_EQ(get_next_line(iss), "5,3 4 6 7 8,6.4");
    EXPECT_EQ(get_next_line(iss), "4,2 3 7 8,7.3");
}
