#pragma once

#include <memory>
#include <assert.h>
#include <string>
#include "item.h"
#include "enums.h"


// The utils namespace provides utility functions that assist in the creation
// and management of various objects throughout the project.
// It mainly offers a custom implementation of make_unique, along with helper
// functions for creating instances of the Item class based on different initialization requirements.

namespace utils {

    // make_unique
    // A utility function that replicates the functionality of std::make_unique,
    // which was introduced in C++14. It ensures type safety and prevents the
    // misuse of raw pointers by encouraging the use of smart pointers.
    // Arrays are explicitly not supported to maintain consistency and safety.

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args)
    {
        static_assert(!std::is_array<T>::value, "arrays not supported");
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
    //TODO(Jenna):
    // createItem (Basic version)
    // Creates an Item instance using only an ItemID.
    // This version initializes the Item with default placeholder values such as
    // the name "Item" and the description.
    // mainly used when no specific information is available at creation time.

    inline std::unique_ptr<Item> createItem(ItemID id) {
        return utils::make_unique<Item>(id);
    }

    //create Item  (Overloaded version)
    // Creates an Item instance using detailed initialization parameters including
    // the name, description, and a property value which typically represents rarity.
    // This version is particularly useful when data is sourced externally, such as
    // from a CSV file or user input, and allows for more meaningful Item objects.

    inline std::unique_ptr<Item> createItem(ItemID id, const std::string& name, const std::string& descrription, int property) {
        return utils::make_unique<Item>(id, name, description, property);
    }
}
