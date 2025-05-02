#pragma once

#include "enums.h"
#include "item.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// The utils namespace provides utility functions that assist in the creation
// and management of various objects throughout the project.
// It mainly offers a custom implementation of make_unique, along with helper
// functions for creating instances of the Item class based on different
// initialization requirements.

// Fix: Update path to point to correct location
const std::string ITEM_DATA = "./src/data/items.bsv";

namespace utils {

// make_unique
// A utility function that replicates the functionality of std::make_unique,
// which was introduced in C++14. It ensures type safety and prevents the
// misuse of raw pointers by encouraging the use of smart pointers.
// Arrays are explicitly not supported to maintain consistency and safety.

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

inline std::unique_ptr<Item> createItem(int id, const std::string &name,
                                        const std::string &description,
                                        int rarity, bool hasCustomBehavior,
                                        const std::vector<int> &flatBonus,
                                        const std::vector<float> &mult) {
    return utils::make_unique<Item>(id, name, description, rarity,
                                    hasCustomBehavior, flatBonus, mult);
}

inline void
loadItems(std::vector<std::vector<std::unique_ptr<Item> > > &unobtainedItems) {
    // Resize to hold all rarity levels (0-3)
    unobtainedItems.resize(4);

    std::ifstream itemData(ITEM_DATA);
    if (!itemData.is_open()) {
        std::cerr << "Failed to open item data file: " << ITEM_DATA
                  << std::endl;
        return;
    }

    std::string line, name, temp, desc;
    int id, rarity;
    bool hasCustomBehavior;

    while (std::getline(itemData, line)) {
        std::stringstream ss(line);
        std::vector<int> flatBonus;
        std::vector<float> mult;

        // Get ID
        std::getline(ss, temp, '|');
        id = atoi(temp.c_str());

        // Get Name
        std::getline(ss, name, '|');

        // Get Description
        std::getline(ss, desc, '|');

        // Get rarity
        std::getline(ss, temp, '|');
        rarity = atoi(temp.c_str());

        // Validate rarity
        if (rarity < 0 || rarity >= unobtainedItems.size()) {
            std::cerr << "Invalid rarity " << rarity << " for item " << name
                      << std::endl;
            continue;
        }

        // Get hasCustomBehavior
        std::getline(ss, temp, '|');
        hasCustomBehavior = bool(atoi(temp.c_str()));

        // Get the 5 flat bonuses
        flatBonus.reserve(5);
        for (int i = 0; i < 5; i++) {
            std::getline(ss, temp, '|');
            flatBonus.push_back(atoi(temp.c_str()));
        }

        // Get the 5 mults
        mult.reserve(5);
        for (int i = 0; i < 5; i++) {
            std::getline(ss, temp, '|');
            mult.push_back(atof(temp.c_str()));
        }

        unobtainedItems[rarity].push_back(utils::createItem(
            id, name, desc, rarity, hasCustomBehavior, flatBonus, mult));
    }
}
} // namespace utils
