#pragma once

#include <memory>
#include <assert.h>
#include <string>
#include "item.h"
#include "enums.h"

namespace utils {

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args)
    {
        static_assert(!std::is_array<T>::value, "arrays not supported");
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
    inline std::unique_ptr<Item> createItem(ItemID id) {
        return utils::make_unique<Item>(id);
    }
    inline std::unique_ptr<Item> createItem(ItemID id, const std::string& name, const std::string& descrription, int property) {
        return utils::make_unique<Item>(id, name, description, property);
    }
}
