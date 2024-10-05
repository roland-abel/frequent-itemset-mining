/// @file test_utilities.h
/// @brief Helper functions for testing.
///
/// @author Roland Abel
/// @date October 03, 2024
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

#ifndef _TEST_UTILITIES_H
#define _TEST_UTILITIES_H

#include <vector>
#include <algorithm>

#define EXPECT_LIST_EQ(list1, list2) \
    do { \
        auto it1 = (list1).begin(); \
        auto it2 = (list2).begin(); \
        while (it1 != (list1).end() && it2 != (list2).end()) { \
            EXPECT_EQ(*it1, *it2) << "Element mismatch at index " << std::distance((list1).begin(), it1); \
            ++it1; \
            ++it2; \
        } \
        EXPECT_EQ(it1, (list1).end()) << "First list has more elements."; \
        EXPECT_EQ(it2, (list2).end()) << "Second list has more elements."; \
    } while (0)

#endif // _TEST_UTILITIES_H