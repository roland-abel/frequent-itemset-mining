/// @file apriori_tests.cpp
/// @brief Unit test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date December 8, 2023
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
#include "utils.h"

using namespace fim;
using namespace std::chrono;

class UtilsTests : public ::testing::Test {
};

TEST(UtilsTests, AlgorithmToStringTest) {
    EXPECT_EQ(to_string(algorithm_t::APRIORI), "apriori");
    EXPECT_EQ(to_string(algorithm_t::FP_GROWTH), "fp-growth");
    EXPECT_EQ(to_string(algorithm_t::UNKNOWN), "unknown");
}

TEST(UtilsTests, AlgorithmToEnumTest) {
    EXPECT_EQ(to_algorithm("apriori"), algorithm_t::APRIORI);
    EXPECT_EQ(to_algorithm("fp-growth"), algorithm_t::FP_GROWTH);
    EXPECT_EQ(to_algorithm("X"), algorithm_t::UNKNOWN);
}

TEST(UtilsTests, Iso8601DatetimeTest) {
    const auto dt = to_datetime(year{2024}, month{9}, day{1}, hours{14}, minutes{30}, seconds{5});
    EXPECT_EQ(iso8601_datetime(dt), "2024-09-01T14:30:05Z");
}

