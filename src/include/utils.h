#pragma once

#include <memory>
#include <assert.h>

namespace utils{

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args)
    {
        static_assert(!std::is_array<T>::value, "arrays not supported");
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }


}