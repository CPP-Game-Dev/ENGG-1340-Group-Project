
#include "include/item.h"
#include "include/player.h"
#include "include/enums.h"
#include "include/utils.h"


#include <string>

// Default constructor for item
// Initializes an Item with default placeholder values.
// This constructor sets the name to "Item", description to "Description",
// rarity to 0, and initializes all bonus attributes to zero.
// It is primarily used when creating a generic Item without specific data.
Item::Item(ItemID id) {

    // implementation of the item class default constructor

    // implementation of the item class overloaded constructor
    // in the order of (id, name, description, property)

    this->id = id;

    this->name = "Item";
    this->description = "Description";
    this->rarity = 0;

    this->counter = 0;

    this->bonusStaminaMax = 0;
    this->bonusRationRegen = 0;
    this->bonusFov = 0;
    this->bonusRationCapacity = 0;
    this->bonusPickaxeCapacity = 0;

    this->bonusStaminaMaxMult = 0.0;
    this->bonusRationRegenMult = 0.0;
    this->bonusFovMult = 0.0;
    this->bonusRationCapacityMult = 0.0;
    this->bonusPickaxeCapacityMult = 0.0;
}

// Overloaded constructor for Item
// Initializes an Item using the provided name, description, and property.
// The 'property' parameter is used to set the item's rarity.
// This constructor allows direct initialization of meaningful Item data,
// typically used when creating Items based on external input like a CSV file.
Item::Item(ItemID id, const std::string &name, const std::string &description,
           int property) {
    this->id = id;
    this->name = name;
    this->description = description;
    this->rarity = property;

    this->counter = 0;

    this->bonusStaminaMax = 0;
    this->bonusRationRegen = 0;
    this->bonusFov = 0;
    this->bonusRationCapacity = 0;
    this->bonusPickaxeCapacity = 0;

    this->bonusStaminaMaxMult = 0.0;
    this->bonusRationRegenMult = 0.0;
    this->bonusFovMult = 0.0;
    this->bonusRationCapacityMult = 0.0;
    this->bonusPickaxeCapacityMult = 0.0;
}
