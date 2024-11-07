/// @file writer.h
/// @brief
///
/// @author Roland Abel
/// @date August 31, 2024
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

#pragma once

#include "utils.h"

namespace fim::data {

// CSV
//    itemset_length,itemset,support,transaction_count
//    1,"Brot",0.60,200
//    1,"Butter",0.50,180
//    2,"Brot, Butter",0.45,150
//    2,"Milch, Butter",0.40,135
//    3,"Brot, Milch, Butter",0.30,100

// JSON
//{
//    "frequent_itemsets": [
//{
//    "itemset": ["Brot", "Butter"],
//    "support": 0.45,
//    "transaction_count": 150
//},
//{
//"itemset": ["Milch", "Butter"],
//"support": 0.40,
//"transaction_count": 135
//},
//{
//"itemset": ["Brot", "Milch", "Butter"],
//"support": 0.30,
//"transaction_count": 100
//},
//{
//"itemset": ["Käse", "Wurst"],
//"support": 0.35,
//"transaction_count": 120
//}
//]
//}



    ///
    /// @param os
    /// @param result
    /// @return
    std::ostream& operator<<(std::ostream& os, const frequency_output_t& result);

    ///
    /// @param os
    /// @param output
    /// @return
    auto to_json(std::ostream& os, const frequency_output_t &output);

    ///
    /// @param filename
    /// @param output
    /// @return
    auto to_json(const std::string &filename, const frequency_output_t &output) -> io_error_t;

    ///
    /// @param output
    /// @return
    auto to_json(const frequency_output_t &output) -> std::string;

    ///
    /// @param is
    /// @param result
    /// @return
    std::istream& operator>>(std::istream& is, frequency_output_t& result);
}
