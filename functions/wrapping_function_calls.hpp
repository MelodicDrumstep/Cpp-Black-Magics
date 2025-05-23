#pragma once

#include <utility>
#include <functional>

// Use decltype(auto) to return exactly the type of the return value
// (If we just use "auto", it will decay!)
template <typename Callable, typename ... Args>
decltype(auto) call(Callable && op, Args && ... args) {
    // Do something here

    return std::invoke(std::forward<Callable>(op),
                       std::forward<Args>(args)...);
    // Perfect Forwarding
}