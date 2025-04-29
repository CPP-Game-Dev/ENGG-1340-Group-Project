
#include "include/item.h"
#include "player.h"
#include "include/enums.h"
#include "include/utils.h"


#include <string>

/*
 * Default constructor for Item
 *
 * Initializes an Item with placeholder values such as:
 * - name: "Item"
 * - description: "Description"
 * - rarity: 0
 * - all bonuses and multipliers: 0
 *
 * @param id Unique identifier for the item
 * @return none
 */
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

/*
 * Overloaded constructor for Item
 *
 * Initializes an Item with specific values provided by parameters.
 * All bonus stats are initialized to zero. Typically used when reading from file.
 *
 * @param id          Unique identifier for the item
 * @param name        Name of the item
 * @param description Short description of the item
 * @param property    Integer representing item rarity or other property
 * @return none
 */
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
/*
 * Custom item behavior override
 *
 * Defines logic for items that have dynamic effects on the player.
 * Only executed if hasCustomBehavior is set to true.
 * Empty by default — override in derived classes if needed.
 *
 * @param player Reference to the Player object affected by the item
 * @return void
 */
void Item::update(Player& player) {

}
