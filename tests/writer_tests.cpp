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
#include "test_data.h"
#include "writer.h"

using namespace std;
using namespace std::chrono;

using namespace fim;
using namespace fim::itemset;
using namespace fim::data;
using namespace fim::tests;

using frequencies_t = fim::itemset::itemset_count_t; // TODO

class WriterTests : public ::testing::Test {
protected:

    static auto get_frequent_itemsets() -> std::pair<itemsets_t, frequencies_t> {
        const std::set<std::pair<itemset_t, size_t>> itemsets_frequencies = {
                {{Milk,   Bread, Butter}, 63},
                {{Coffee, Milk,  Cheese}, 74},
                {{Coffee, Milk,  Bread},  82},
        };

        itemsets_t itemsets{};
        frequencies_t frequencies{};

        for (const auto &[itemset, frequency]: itemsets_frequencies) {
//            itemsets.insert(itemset);
//            frequencies[fim::hash_code(itemset)] = frequency;
        }
        return {itemsets, frequencies};
    }
};

TEST_F(WriterTests, WriterEmptyTest) {
    const auto creation_data = to_datetime(year{2024}, month{9}, day{1}, hours{14}, minutes{30}, seconds{5});
    const auto &[itemsets, frequencies] = get_frequent_itemsets();

    const frequency_output_t output = {
            itemsets,
            frequencies,
            0.6,
            10,
            100,
            creation_data,
            algorithm_t::APRIORI
    };
    
    std::stringstream ss;
    ss << output;

    frequency_output_t deserialized;
    ss >> deserialized;

    EXPECT_EQ(output.num_items, deserialized.num_items);
    EXPECT_EQ(output.num_transactions, deserialized.num_transactions);
    EXPECT_EQ(output.min_support, deserialized.min_support);
    EXPECT_EQ(output.algorithm, deserialized.algorithm);

    EXPECT_EQ(output.itemsets, deserialized.itemsets);
    EXPECT_EQ(output.frequencies, deserialized.frequencies);

//    EXPECT_TRUE(deserialized.itemsets.contains({Milk, Bread, Butter}));   // TODO
//    EXPECT_TRUE(deserialized.itemsets.contains({Coffee, Milk, Cheese}));
//    EXPECT_TRUE(deserialized.itemsets.contains({Coffee, Milk, Bread}));
//
//    const auto get_frequency = [&](const itemset_t x) {
//        return deserialized.frequencies[hash_code(x)];
//    };

//    EXPECT_EQ(get_frequency({Milk, Bread, Butter}), 63);
//    EXPECT_EQ(get_frequency({Coffee, Milk, Cheese}), 74);
//    EXPECT_EQ(get_frequency({Coffee, Milk, Bread}), 82);

//    {{Milk,   Bread, Butter}, 63},
//    {{Coffee, Milk,  Cheese}, 74},
//    {{Coffee, Milk,  Bread},  82},



    std::cout << output << std::endl;



//    std::cout << to_json(result) << std::endl;

//    fim::io::write(
//            std::cout,
//            itemsets,
//            frequencies,
//            0.75,
//            itemsets.size(),
//            3,
//            creation_data,
//            algorithm_t::APRIORI)
//            << std::endl;


//    std::istringstream iss("");
//
//    const auto &result = read_csv(iss);
//    ASSERT_FALSE(result.has_value());
//    EXPECT_EQ(result.error(), io_error_t::EMPTY_ERROR);

}