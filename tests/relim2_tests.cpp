/// @file relim2_tests.cpp
/// @brief Unit test for the RElim algorithm.
/// @see https://borgelt.net/papers/simple.pdf
/// @see https://borgelt.net/papers/relim.pdf
///
/// @author Roland Abel
/// @date September 29, 2024
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
#include "relim.h"

using namespace fim;
using namespace fim::relim;

using std::views::transform;

class Relim2Tests : public ::testing::Test {
protected:
    static size_t get_min_support() { return 2; }

    static auto get_database() -> database_t {
        return database_t{
                {'a', 'd'},
                {'a', 'c', 'd', 'e'},
                {'b', 'd'},
                {'b', 'c', 'd'},
                {'b', 'c'},
                {'a', 'b', 'd'},
                {'b', 'd', 'e'},
                {'b', 'c', 'd', 'e'},
                {'b', 'c'},
                {'a', 'b', 'd'}
        };
    }
};

TEST_F(Relim2Tests, PreprocessingDatabaseTest) {
    database_t db = get_database();
    const auto item_count = preprocessing(db, get_min_support());

    EXPECT_EQ(item_count.at('e'), 3);
    EXPECT_EQ(item_count.at('a'), 4);
    EXPECT_EQ(item_count.at('c'), 5);
    EXPECT_EQ(item_count.at('b'), 8);
    EXPECT_EQ(item_count.at('d'), 8);

    ASSERT_EQ(db.size(), 10);

    EXPECT_EQ(db[0], itemset_t({'a', 'd'}));
    EXPECT_EQ(db[1], itemset_t({'e', 'a', 'c', 'd'}));
    EXPECT_EQ(db[2], itemset_t({'b', 'd'}));
    EXPECT_EQ(db[3], itemset_t({'c', 'b', 'd'}));
    EXPECT_EQ(db[4], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[5], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[7], itemset_t({'e', 'c', 'b', 'd'}));
    EXPECT_EQ(db[8], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[9], itemset_t({'a', 'b', 'd'}));
}

TEST_F(Relim2Tests, SortLexicographicallyTest) {
    const auto min_support = get_min_support();
    database_t db = get_database();

    const auto count = preprocessing(db, min_support);

    db.sort_lexicographically(count.get_item_comparer());
    ASSERT_EQ(db.size(), 10);

    EXPECT_EQ(db[0], itemset_t({'e', 'a', 'c', 'd'}));
    EXPECT_EQ(db[1], itemset_t({'e', 'c', 'b', 'd'}));
    EXPECT_EQ(db[2], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[3], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[4], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[5], itemset_t({'a', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'c', 'b', 'd'}));
    EXPECT_EQ(db[7], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[8], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[9], itemset_t({'b', 'd'}));
}

TEST_F(Relim2Tests, CreateInitialDatabaseTest) {
    const auto min_support = get_min_support();
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support);
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support);
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    const auto header = conditional_db.header;
    ASSERT_EQ(header.size(), 5);

    EXPECT_EQ(header[0].count, 0);
    EXPECT_EQ(header[0].prefix, 'd');
    EXPECT_EQ(header[0].suffixes.size(), 0);

    EXPECT_EQ(header[1].count, 1);
    EXPECT_EQ(header[1].prefix, 'b');
    EXPECT_EQ(header[1].suffixes.size(), 1);

    EXPECT_EQ(header[2].count, 3);
    EXPECT_EQ(header[2].prefix, 'c');
    EXPECT_EQ(header[2].suffixes.size(), 2);

    EXPECT_EQ(header[3].count, 3);
    EXPECT_EQ(header[3].prefix, 'a');
    EXPECT_EQ(header[3].suffixes.size(), 2);

    EXPECT_EQ(header[4].count, 3);
    EXPECT_EQ(header[4].prefix, 'e');
    EXPECT_EQ(header[4].suffixes.size(), 3);

    auto it = header[1].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = header[2].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b'}));

    it = header[3].suffixes.begin();
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = header[4].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'a', 'c', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'c', 'b', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));
}

TEST_F(Relim2Tests, PrefixDatabaseTest) {
    const auto min_support = get_min_support();
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support);
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support);
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    const auto &prefix_db = conditional_db.get_prefix_database();

    const auto header = prefix_db.header;
    ASSERT_EQ(header.size(), 4);

    EXPECT_EQ(header[0].count, 0);
    EXPECT_EQ(header[0].prefix, 'd');
    EXPECT_EQ(header[0].suffixes.size(), 0);

    EXPECT_EQ(header[1].count, 1);
    EXPECT_EQ(header[1].prefix, 'b');
    EXPECT_EQ(header[1].suffixes.size(), 1);

    EXPECT_EQ(header[2].count, 1);
    EXPECT_EQ(header[2].prefix, 'c');
    EXPECT_EQ(header[2].suffixes.size(), 1);

    EXPECT_EQ(header[3].count, 1);
    EXPECT_EQ(header[3].prefix, 'a');
    EXPECT_EQ(header[3].suffixes.size(), 1);

    auto it = header[1].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = header[2].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    it = header[3].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'c', 'd'}));
}

TEST_F(Relim2Tests, EliminateTest) {
    const auto min_support = get_min_support();
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support);
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support);
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    const auto &prefix_db = conditional_db.get_prefix_database();
    const auto prefix = conditional_db.eliminate(prefix_db);

    EXPECT_EQ(prefix, 'e');

    const auto header = conditional_db.header;
    ASSERT_EQ(header.size(), 4);

    EXPECT_EQ(header[0].count, 0);
    EXPECT_EQ(header[0].prefix, 'd');
    EXPECT_EQ(header[0].suffixes.size(), 0);

    EXPECT_EQ(header[1].count, 2);
    EXPECT_EQ(header[1].prefix, 'b');
    EXPECT_EQ(header[1].suffixes.size(), 1);

    EXPECT_EQ(header[2].count, 4);
    EXPECT_EQ(header[2].prefix, 'c');
    EXPECT_EQ(header[2].suffixes.size(), 2);

    EXPECT_EQ(header[3].count, 4);
    EXPECT_EQ(header[3].prefix, 'a');
    EXPECT_EQ(header[3].suffixes.size(), 3);

    auto it = header[1].suffixes.begin();
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = header[2].suffixes.begin();
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b'}));

    it = header[3].suffixes.begin();
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'c', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    ++it;
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));
}








TEST_F(Relim2Tests, RelimAlgorithmTest) {
//    database_t db = get_database();
//    const auto min_support = get_min_support();
//
//    const auto item_count = preprocessing(db, min_support);
//    db.sort_lexicographically(item_count.get_item_comparer());
//
//    const auto frequent_items = item_count.get_frequent_items(min_support);
//    const auto &itemsets = relim_algorithm(db, frequent_items, min_support).sort_each_itemset();


}

