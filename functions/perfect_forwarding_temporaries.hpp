#pragma once

#include <utility>
#include <functional>

template <typename T, typename CallableGet, typename CallableSet>
void foo(T x, CallableGet && get, CallableSet && set) {
    auto && val = get(x);
    set(std::forward<decltype(val)>(val));
}