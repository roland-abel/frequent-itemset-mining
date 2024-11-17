/// @file relim_tests.cpp
/// @brief Unit test for the RElim algorithm.
/// @see https://borgelt.net/papers/simple.pdf
/// @see https://borgelt.net/papers/relim.pdf
///
/// @author Roland Abel
/// @date September 29, 2024
///
/// Copyright (c) 2024 Roland Abel
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
#include <ranges>
#include "relim.h"
#include "item_counts.h"

using namespace fim;
using namespace fim::algorithm::relim;

using std::views::transform;
using std::ranges::to;

class RelimTests : public testing::Test {
protected:
    static size_t min_support() { return 2; }

    static auto get_database() -> database_t {
        return database_t{
            {'a', 'd'},
            {'a', 'c', 'd', 'e'},
            {'b', 'd'},
            {'b', 'c', 'd'},
            {'b', 'c', 'f'},
            {'a', 'b', 'd'},
            {'b', 'd', 'e'},
            {'b', 'c', 'd', 'e'},
            {'g', 'b', 'c'},
            {'a', 'b', 'd'}
        };
    }

    static auto get_prefix(const header_element_t &h) -> item_t { return h.prefix; }
    static auto get_count(const header_element_t &h) -> size_t { return h.count; }
    static auto get_suffixes(const header_element_t &h) -> suffixes_t { return h.suffixes; }
};

TEST_F(RelimTests, ItemComparerTest) {
    const auto &counts = get_database().get_item_counts();
    const auto compare = counts.get_item_compare();

    // item order: e < a < c < b < d
    EXPECT_EQ(counts.at('a'), 4);
    EXPECT_EQ(counts.at('b'), 8);
    EXPECT_EQ(counts.at('c'), 5);
    EXPECT_EQ(counts.at('d'), 8);
    EXPECT_EQ(counts.at('e'), 3);
    EXPECT_EQ(counts.at('f'), 1);
    EXPECT_EQ(counts.at('g'), 1);

    EXPECT_TRUE(compare('e', 'a'));
    EXPECT_TRUE(compare('e', 'c'));
    EXPECT_TRUE(compare('e', 'b'));
    EXPECT_TRUE(compare('e', 'd'));
    EXPECT_TRUE(compare('a', 'c'));
    EXPECT_TRUE(compare('a', 'b'));
    EXPECT_TRUE(compare('a', 'd'));
    EXPECT_TRUE(compare('c', 'b'));
    EXPECT_TRUE(compare('c', 'd'));

    EXPECT_FALSE(compare('d', 'c'));
    EXPECT_FALSE(compare('d', 'a'));
    EXPECT_FALSE(compare('d', 'e'));
    EXPECT_FALSE(compare('b', 'a'));
    EXPECT_FALSE(compare('b', 'e'));
    EXPECT_FALSE(compare('c', 'a'));
    EXPECT_FALSE(compare('c', 'e'));
    EXPECT_FALSE(compare('a', 'e'));
}

TEST_F(RelimTests, LexicographicalCompareTest) {
    const auto &counts = get_database().get_item_counts();
    const auto compare = counts.get_item_compare();

    auto compare_itemsets = [&](const auto &x, const auto &y) -> bool {
        return lexicographical_compare(x, y, compare);
    };

    EXPECT_TRUE(compare_itemsets(itemset_t{'a', 'd'}, itemset_t{'a'}));
    EXPECT_TRUE(compare_itemsets(itemset_t{'e', 'd'}, itemset_t{'a'}));
    EXPECT_TRUE(compare_itemsets(itemset_t{'e', 'd'}, itemset_t{'a'}));
    EXPECT_TRUE(compare_itemsets(itemset_t{'a', 'c'}, itemset_t{'a', 'b'}));
    EXPECT_TRUE(compare_itemsets(itemset_t{'a', 'b'}, itemset_t{'a', 'd'}));

    EXPECT_FALSE(compare_itemsets(itemset_t{'a', 'c'}, itemset_t{'a', 'c', 'b'}));
    EXPECT_FALSE(compare_itemsets(itemset_t{'e', 'b', 'c'}, itemset_t{'e', 'b', 'c'}));
    EXPECT_FALSE(compare_itemsets(itemset_t{'c', 'b', 'd'}, itemset_t{'c', 'b', 'a'}));
}

TEST_F(RelimTests, AddSuffixItemsetTest) {
    const auto &counts = get_database().get_item_counts();
    const auto compare = counts.get_item_compare();

    suffixes_t suffixes{};

    auto add_itemset = [&](const itemset_t &suffix) -> void {
        // item order: e < a < c < b < d
        suffixes.add_itemset(suffix, compare);
    };

    add_itemset(itemset_t{'a', 'd'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'b', 'a'});
    add_itemset(itemset_t{'b', 'e', 'a'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'a', 'e', 'b'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'a', 'd'});
    add_itemset(itemset_t{'b', 'd'});

    auto it = suffixes.begin();

    EXPECT_EQ(it->itemset, (itemset_t{'e', 'a', 'b'}));
    EXPECT_EQ(it->count, 2);

    EXPECT_EQ((++it)->itemset, (itemset_t{'a', 'b'}));
    EXPECT_EQ(it->count, 1);

    EXPECT_EQ((++it)->itemset, (itemset_t{'a', 'd'}));
    EXPECT_EQ(it->count, 2);

    EXPECT_EQ((++it)->itemset, (itemset_t{'b', 'd'}));
    EXPECT_EQ(it->count, 1);

    EXPECT_EQ((++it)->itemset, itemset_t{'d'});
    EXPECT_EQ(it->count, 4);
}

TEST_F(RelimTests, SortLexicographicallyTest) {
    const auto &[db, counts] = get_database().transaction_reduction(min_support());
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

TEST_F(RelimTests, CreateInitialDatabaseTest) {
    const auto &[db, item_counts] = get_database().transaction_reduction(min_support());
    const auto compare = item_counts.get_item_compare();

    const auto &freq_items = item_counts.get_frequent_items(min_support());
    const auto &conditional_db = conditional_database_t::create_initial_database(db, freq_items, compare);

    const auto &header = conditional_db.header;
    ASSERT_EQ(header.size(), 5);

    const itemset_t &prefixes = header | transform(get_prefix) | to<itemset_t>();
    EXPECT_EQ(prefixes, itemset_t({'d', 'b', 'c', 'a', 'e'}));

    const counts_t &prefix_counts = header | transform(get_count) | to<counts_t>();
    EXPECT_EQ(prefix_counts, counts_t({0, 1, 3, 3, 3}));

    const auto &suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();
    ASSERT_EQ(suffixes[0].size(), 0);
    ASSERT_EQ(suffixes[1].size(), 1);
    ASSERT_EQ(suffixes[2].size(), 2);
    ASSERT_EQ(suffixes[3].size(), 2);
    ASSERT_EQ(suffixes[4].size(), 3);

    auto it = suffixes[1].begin();
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = suffixes[2].begin();
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'b'}));

    it = suffixes[3].begin();
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'d'}));

    it = suffixes[4].begin();
    EXPECT_EQ(it->itemset, itemset_t({'a', 'c', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'c', 'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'b', 'd'}));
}

TEST_F(RelimTests, GetPrefixDatabaseTest) {
    const auto &[db, counts] = get_database().transaction_reduction(min_support());
    const auto compare = counts.get_item_compare();

    const auto freq_items = counts.get_frequent_items(min_support());
    const auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, compare);

    const auto &prefix_db = conditional_db.get_prefix_database();

    const auto header = prefix_db.header;
    ASSERT_EQ(header.size(), 4);

    const itemset_t &prefixes = header | transform(get_prefix) | to<itemset_t>();
    EXPECT_EQ(prefixes, itemset_t({'d', 'b', 'c', 'a'}));

    const counts_t &prefix_counts = header | transform(get_count) | to<counts_t>();
    EXPECT_EQ(prefix_counts, counts_t({0, 1, 1, 1}));

    const auto &suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();

    ASSERT_EQ(suffixes[0].size(), 0);
    ASSERT_EQ(suffixes[1].size(), 1);
    ASSERT_EQ(suffixes[2].size(), 1);
    ASSERT_EQ(suffixes[3].size(), 1);

    auto it = suffixes[1].begin();
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = suffixes[2].begin();
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    it = suffixes[3].begin();
    EXPECT_EQ(it->itemset, itemset_t({'c', 'd'}));
}

TEST_F(RelimTests, EliminateTest) {
    const auto &[db, counts] = get_database().transaction_reduction(min_support());
    const auto compare = counts.get_item_compare();

    const auto freq_items = counts.get_frequent_items(min_support());
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, compare);

    // eliminated prefix 'e'
    auto prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'e');

    auto header = conditional_db.header;
    ASSERT_EQ(header.size(), 4);

    EXPECT_EQ(header | transform(get_prefix) | to<itemset_t>(), itemset_t({'d', 'b', 'c', 'a'}));
    EXPECT_EQ(header | transform(get_count) | to<counts_t>(), counts_t({0, 2, 4, 4}));

    auto suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();
    ASSERT_EQ(suffixes[0].size(), 0);
    ASSERT_EQ(suffixes[1].size(), 1);
    ASSERT_EQ(suffixes[2].size(), 2);
    ASSERT_EQ(suffixes[3].size(), 3);

    auto it = suffixes[1].begin();
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = suffixes[2].begin();
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'b'}));

    it = suffixes[3].begin();
    EXPECT_EQ(it->itemset, itemset_t({'c', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'d'}));

    // eliminated prefix 'a'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'a');

    header = conditional_db.header;
    ASSERT_EQ(header.size(), 3);

    EXPECT_EQ(header | transform(get_prefix) | to<itemset_t>(), itemset_t({'d', 'b', 'c'}));
    EXPECT_EQ(header | transform(get_count) | to<counts_t>(), counts_t({1, 4, 5}));

    suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();
    ASSERT_EQ(suffixes[0].size(), 0);
    ASSERT_EQ(suffixes[1].size(), 1);
    ASSERT_EQ(suffixes[2].size(), 3);

    it = suffixes[1].begin();
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    it = suffixes[2].begin();
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'b'}));
    EXPECT_EQ((++it)->itemset, itemset_t({'d'}));

    // eliminated prefix 'c'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'c');

    header = conditional_db.header;
    ASSERT_EQ(header.size(), 2);

    EXPECT_EQ(header | transform(get_prefix) | to<itemset_t>(), itemset_t({'d', 'b'}));
    EXPECT_EQ(header | transform(get_count) | to<counts_t>(), counts_t({2, 8}));

    suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();
    ASSERT_EQ(suffixes[0].size(), 0);
    ASSERT_EQ(suffixes[1].size(), 1);

    it = suffixes[1].begin();
    EXPECT_EQ(it->itemset, itemset_t({'d'}));

    // eliminated prefix 'b'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'b');

    header = conditional_db.header;
    ASSERT_EQ(header.size(), 1);

    EXPECT_EQ(header | transform(get_prefix) | to<itemset_t>(), itemset_t({'d'}));
    EXPECT_EQ(header | transform(get_count) | to<counts_t>(), counts_t({8}));

    suffixes = header | transform(get_suffixes) | to<std::vector<suffixes_t> >();
    ASSERT_EQ(suffixes[0].size(), 0);
}

TEST_F(RelimTests, RelimAlgorithmTest) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto compare = item_counts.get_item_compare();

    const auto &freq_items = relim_algorithm(db, min_support()).sort_each_itemset(compare);
    const auto &counts = itemset_counts_t::create_itemset_counts(db, freq_items, compare);

    ASSERT_EQ(freq_items.size(), 17);
    ASSERT_EQ(counts.size(), 17);

    auto verify = [&](const itemset_t &itemset, const size_t support = 0) -> bool {
        return freq_items.contains(itemset.sort_itemset(compare)) && counts.get_count(itemset) == support;
    };

    EXPECT_TRUE(verify({'a'}, 4));
    EXPECT_TRUE(verify({'b'}, 8));
    EXPECT_TRUE(verify({'d'}, 8));
    EXPECT_TRUE(verify({'e'}, 3));
    EXPECT_TRUE(verify({'c'}, 5));

    EXPECT_TRUE(verify({'a', 'd'}, 4));
    EXPECT_TRUE(verify({'c', 'b'}, 4));
    EXPECT_TRUE(verify({'e', 'c'}, 2));
    EXPECT_TRUE(verify({'e', 'b'}, 2));
    EXPECT_TRUE(verify({'e', 'd'}, 3));
    EXPECT_TRUE(verify({'a', 'b'}, 2));
    EXPECT_TRUE(verify({'c', 'd'}, 3));
    EXPECT_TRUE(verify({'b', 'd'}, 6));

    EXPECT_TRUE(verify({'e', 'b', 'd'}, 2));
    EXPECT_TRUE(verify({'a', 'b', 'd'}, 2));
    EXPECT_TRUE(verify({'e', 'c', 'd'}, 2));
    EXPECT_TRUE(verify({'c', 'b', 'd'}, 2));
}
