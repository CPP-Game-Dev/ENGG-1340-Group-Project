#pragma once

#include <memory>
#include <assert.h>

namespace utils {

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args)
    {
        static_assert(!std::is_array<T>::value, "arrays not supported");
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    void shuffle(std::vector<T>& array) {
        for (size_t i = array.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(array[i], array[j]); 
        }
    }
}
