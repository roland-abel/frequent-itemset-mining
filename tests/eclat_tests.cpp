/// @file eclat_tests.cpp
/// @brief Unit test for the Eclat algorithm.
///
/// @author Roland Abel
/// @date September 8, 2024
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
#include <ranges>
#include "eclat.h"

using namespace fim;
using namespace fim::algorithm::eclat;

using std::views::transform;

class EclatTests : public testing::Test {
protected:
    static database_t get_database() {
        return database_t{
            {1, 3, 4, 2, 6, 7, 8},
            {3, 4, 2, 5, 6, 7},
            {1, 4, 5, 6, 7},
            {1, 4},
            {1, 4, 5},
            {1, 7},
            {1, 3, 4, 2, 5, 6, 7},
            {8},
            {1, 3, 4, 6},
            {1, 3, 2, 5, 6, 7}
        };
    }
};

TEST_F(EclatTests, ToVerticalTransactionsTest) {
    const auto &db = get_database();
    const auto &vertical_db = to_vertical_database(db);

    ASSERT_EQ(db.size(), 10);
    ASSERT_EQ(vertical_db.size(), 8);

    EXPECT_EQ(vertical_db.at(1), (tidset_t{0, 2, 3, 4, 5, 6, 8, 9}));
    EXPECT_EQ(vertical_db.at(2), (tidset_t{0, 1, 6, 9}));
    EXPECT_EQ(vertical_db.at(3), (tidset_t{0, 1, 6, 8, 9}));
    EXPECT_EQ(vertical_db.at(4), (tidset_t{0, 1, 2, 3, 4, 6, 8}));
    EXPECT_EQ(vertical_db.at(5), (tidset_t{1, 2, 4, 6, 9}));
    EXPECT_EQ(vertical_db.at(6), (tidset_t{0, 1, 2, 6, 8, 9}));
    EXPECT_EQ(vertical_db.at(7), (tidset_t{0, 1, 2, 5, 6, 9}));
    EXPECT_EQ(vertical_db.at(8), (tidset_t{0, 7}));
}

TEST_F(EclatTests, SetIntersectionOfTidsTest) {
    EXPECT_EQ(set_intersection({}, {}), (tidset_t{}));
    EXPECT_EQ(set_intersection({}, {0, 1, 2, 3, 6}), (tidset_t{}));
    EXPECT_EQ(set_intersection({0, 1, 2, 3}, {3, 4, 5, 6}), (tidset_t{3}));
    EXPECT_EQ(set_intersection({0, 3, 2, 3, 6, 6}, {0, 3, 2, 3, 6}), (tidset_t{0, 3, 2, 6}));
    EXPECT_EQ(set_intersection({0, 1, 3, 5, 9}, {0, 1, 2, 3, 6, 8, 9}), (tidset_t{0, 1, 3, 9}));
}
