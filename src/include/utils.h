#pragma once

#include <memory>
#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "item.h"
#include "enums.h"


// The utils namespace provides utility functions that assist in the creation
// and management of various objects throughout the project.
// It mainly offers a custom implementation of make_unique, along with helper
// functions for creating instances of the Item class based on different initialization requirements.

const std::string ITEM_DATA = "data/items.bsv";

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

    inline std::unique_ptr<Item> createItem(int id, const std::string& name, const std::string& description, int rarity, bool hasCustomBehavior, const std::vector<int> &flatBonus, const std::vector<float> &mult) {
        return utils::make_unique<Item>(id, name, description, rarity, hasCustomBehavior, flatBonus, mult);
    }

    inline void loadItems(std::vector<std::vector<std::unique_ptr<Item> > > &unobtainedItems) {
        std::ifstream itemData(ITEM_DATA);
        std::string line, name, temp, desc;
        int id, rarity;
        bool hasCustomBehavior;
        std::vector<int> flatBonus(5, 0);
        std::vector<float> mult(5, 0.0f);
        std::unique_ptr<Item> item;

        while(std::getline(itemData, line)) {
            std::stringstream ss(line);

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

            // Get hasCustomBehavior
            std::getline(ss, temp, '|');
            hasCustomBehavior = bool(atoi(temp.c_str()));

            // Get the 5 flat bonuses
            for(int i = 0; i < 5; i++) {
                std::getline(ss, temp, '|');
                flatBonus.push_back(atoi(temp.c_str()));
            }

            // Get the 5 mults
            for(int i = 0; i < 5; i++) {
                std::getline(ss, temp, '|');
                mult.push_back(atof(temp.c_str()));
            }
            unobtainedItems[rarity].push_back(std::move(utils::createItem(id, name, desc, rarity, hasCustomBehavior, flatBonus, mult)));
        }
    }
}
