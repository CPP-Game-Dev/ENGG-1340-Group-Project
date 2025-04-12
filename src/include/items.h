#pragma once

#include "utils.h"
#include "iitem.h"
#include "player.h"
#include <memory>
/*
 * This file contains all class declarations for the in game items
 * Note that each item has it's own class
 */


#pragma region Generator Function
// Function to generate an item instance based on input item class name
template <typename ItemType> std::unique_ptr<IItem> createItem() {
    return utils::make_unique<ItemType>();
}
#pragma endregion

// This class serves as an example & base framework as to how items should be
// implemented
class ExampleItem : public IItem {
  private:
    bool hasTriggered;

  public:
    using IItem::IItem;
    ~ExampleItem() = default;

    void setDefaults();

    void update(Player player);
};
