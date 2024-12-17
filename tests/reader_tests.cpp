/// @file reader_tests.cpp
/// @brief Unit test for the reader's function.
///
/// @author Roland Abel
/// @date August 30, 2024.
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
#include <fstream>
#include "itemset.h"
#include "reader.h"

using namespace std;
using namespace fim;
using namespace fim::data;

class ReaderTests : public testing::Test {
};

TEST_F(ReaderTests, ReadCsvEmptyTest) {
    std::istringstream iss("");
    const auto &result = read_csv(iss);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::EMPTY_ERROR);
}

TEST_F(ReaderTests, ReadCsvNoValuesTest) {
    std::istringstream iss("A\nB\nC\n");
    const auto &result = read_csv(iss, read_csv_config_t{3, ' '});

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::EMPTY_ERROR);
}

TEST_F(ReaderTests, ReadCsvInvalidFormatTest) {
    std::istringstream iss("1 2 3\nx y z\n1 2 3 4");

    const auto &result = read_csv(iss);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::INVALID_FORMAT);
}

TEST_F(ReaderTests, ReadCsvBadErrorTest) {
    std::istringstream iss;
    iss.setstate(std::ios::badbit);

    const auto &result = read_csv(iss);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::UNKNOWN_ERROR);
}

TEST_F(ReaderTests, ReadCsvFailErrorTest) {
    std::ifstream file;
    file.setstate(std::ios::failbit);

    const auto &result = read_csv(file);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), io_error_t::UNKNOWN_ERROR);
}

TEST_F(ReaderTests, ReadCsvReadSimpleTransactionsTest) {
    std::istringstream iss("1 2 3\n1 2\n1 2 3 4");

    const auto &result = read_csv(iss);
    ASSERT_TRUE(result.has_value());

    const auto &db = result.value();
    ASSERT_EQ(db.size(), 3);

    EXPECT_EQ(db[0], itemset_t({1, 2, 3}));
    EXPECT_EQ(db[1], itemset_t({1, 2}));
    EXPECT_EQ(db[2], itemset_t({1, 2, 3, 4}));
}

TEST_F(ReaderTests, ReadCsvTransactionsTest) {
    std::string csv = "Sample db (CSV)\n"
            "1,2,3,4,6,7,8\n"
            "2,3,4,5,6,7\n"
            "1,4,5,6,7\n"
            "1,4\n"
            "1,4,5\n"
            "1,7\n"
            "1,2,3,4,5,6,7\n"
            "8\n"
            "1,3,4,6\n"
            "1,2,3,5,6,7\n"
            "";

    std::istringstream iss(csv);

    const auto &result = read_csv(iss, read_csv_config_t{1, ','});
    ASSERT_TRUE(result.has_value());

    const auto &db = result.value();
    ASSERT_EQ(db.size(), 10);

    EXPECT_EQ(db[0], itemset_t({1, 2, 3, 4, 6, 7, 8}));
    EXPECT_EQ(db[1], itemset_t({2, 3, 4, 5, 6, 7}));
    EXPECT_EQ(db[2], itemset_t({1, 4, 5, 6, 7}));
    EXPECT_EQ(db[3], itemset_t({1, 4}));
    EXPECT_EQ(db[4], itemset_t({1, 4, 5}));
    EXPECT_EQ(db[5], itemset_t({1, 7}));
    EXPECT_EQ(db[6], itemset_t({1, 2, 3, 4, 5, 6, 7}));
    EXPECT_EQ(db[7], itemset_t({8}));
    EXPECT_EQ(db[8], itemset_t({1, 3, 4, 6}));
    EXPECT_EQ(db[9], itemset_t({1, 2, 3, 5, 6, 7}));
}

TEST_F(ReaderTests, ReadCsvFromFileTest) {
    constexpr auto config = read_csv_config_t{0, ','};
    EXPECT_TRUE(read_csv("./data/data_01.csv", config).has_value());
}
