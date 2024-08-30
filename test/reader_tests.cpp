/// @file reader_tests.cpp
/// @brief Unit test for the read transactions function.
///
/// @author Roland Abel
/// @date August 30, 2024
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
#include "test_data.h"
#include "reader.h"

using namespace std;
using namespace rules::io;
using namespace rules::tests;

using itemset_t = rules::itemset_t;

class ReaderTests : public ::testing::Test {
protected:
};

auto get_input() -> std::string {
    std::ostringstream os;

    const auto &transactions = get_transactions();
    for (const auto &itemset: transactions) {
        std::copy(itemset.begin(), itemset.end(), std::ostream_iterator<size_t>(os, " "));
        os << std::endl;
    }
    return os.str();
}

TEST_F(ReaderTests, ReadEmptyTest) {
    std::istringstream is("");

    const auto &result = read_transactions(is);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), error_t::EMPTY_ERROR);
}

TEST_F(ReaderTests, InvalidFormatTest) {
    std::istringstream is("1 2 3\nx y z\n1 2 3 4");

    const auto &result = read_transactions(is);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), error_t::INVALID_FORMAT);
}

TEST_F(ReaderTests, BadErrorTest) {
    std::istringstream is;
    is.setstate(std::ios::badbit);

    const auto &result = read_transactions(is);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), error_t::UNKNOWN_ERROR);
}

TEST_F(ReaderTests, FailErrorTest) {
    std::ifstream file;
    file.setstate(std::ios::failbit);

    const auto &result = read_transactions(file);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), error_t::UNKNOWN_ERROR);
}

TEST_F(ReaderTests, ReadSimpleTransactionsTest) {
    std::istringstream is("1 2 3\n1 2\n1 2 3 4");

    const auto &result = read_transactions(is);
    ASSERT_TRUE(result.has_value());

    const auto &transactions = result.value();
    ASSERT_EQ(transactions.size(), 3);

    EXPECT_EQ(transactions[0], itemset_t({1, 2, 3}));
    EXPECT_EQ(transactions[1], itemset_t({1, 2}));
    EXPECT_EQ(transactions[2], itemset_t({1, 2, 3, 4}));
}

TEST_F(ReaderTests, ReadTransactionsTest) {
    std::istringstream is(get_input());

    const auto &result = read_transactions(is);
    ASSERT_TRUE(result.has_value());

    const auto &transactions = result.value();
    ASSERT_EQ(transactions.size(), 10);

    EXPECT_EQ(transactions[0], itemset_t({Milk, Cheese, Butter, Bread, Sugar, Flour, Cream}));
    EXPECT_EQ(transactions[1], itemset_t({Cheese, Butter, Bread, Coffee, Sugar, Flour}));
    EXPECT_EQ(transactions[2], itemset_t({Milk, Butter, Coffee, Sugar, Flour}));
    EXPECT_EQ(transactions[3], itemset_t({Milk, Butter}));
    EXPECT_EQ(transactions[4], itemset_t({Milk, Butter, Coffee}));
    EXPECT_EQ(transactions[5], itemset_t({Milk, Flour}));
    EXPECT_EQ(transactions[6], itemset_t({Milk, Cheese, Butter, Bread, Coffee, Sugar, Flour}));
    EXPECT_EQ(transactions[7], itemset_t({Cream}));
    EXPECT_EQ(transactions[8], itemset_t({Milk, Cheese, Butter, Sugar}));
    EXPECT_EQ(transactions[9], itemset_t({Milk, Cheese, Bread, Coffee, Sugar, Flour}));
}
