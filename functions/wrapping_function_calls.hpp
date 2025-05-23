#pragma once

#include <utility>
#include <functional>

// Use decltype(auto) to return exactly the type of the return value
// (If we just use "auto", it will decay!)
template <typename Callable, typename ... Args>
decltype(auto) call(Callable && op, Args && ... args) {
    if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
        std::invoke(std::forward<Callable>(op),
                    std::forward<Args>(args)...);
        // Do Something here
        return;
    }
    else {
        decltype(auto) result = std::invoke(std::forward<Callable>(op),
                                            std::forward<Args>(args)...);
        // Do something here
        return result;
    }
}