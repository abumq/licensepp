//
//  Bismillah ar-Rahmaan ar-Raheem
//
//  TestX 1.0.1
//  Single header, header only helper for creating test data
//
//  Copyright (c) 2017 @abumq (Majid Q.)
//
//  This library is released under the MIT Licence.
//
//  https://github.com/abumq/testx
//
#ifndef ABUMQ_TEST_X_H
#define ABUMQ_TEST_X_H

#include <tuple>
#include <vector>

namespace abumq {
namespace testx {

template <typename... T>
using TestData = const std::vector<std::tuple<T...>>;

template <typename... T>
std::tuple<T...> TestCase(T... f) {
    return std::make_tuple(f...);
}

} // namespace testx
} // namespace abumq

#endif // ABUMQ_TEST_X_H
