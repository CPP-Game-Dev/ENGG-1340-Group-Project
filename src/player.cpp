#include "include/player.h"
#include "include/item.h"
#include "include/vector2d.h"
// #include <iostream>
#include <algorithm>

/*
 * Constructor for default player
 * Initializes player with default base stats, multipliers,
 * and starting position (0, 0).
 *
 * Usage:
 * Player player;
 */
Player::Player() {
    resetStats();
}
/*
 * Constructor with custom base stats and inventory
 * Initializes player with provided stats, position, and item list.
 *
 * Usage:
 * Player player(100, 5, 3, 10, 2, Vector2D(1, 1), std::move(items));
 *
 * @param baseStaminaMax      Initial base stamina max
 * @param baseRationRegen     Initial base ration regen
 * @param baseFov             Initial field of view
 * @param baseRationCapacity  Initial base ration capacity
 * @param basePickaxeCapacity Initial base pickaxe capacity
 * @param pos                 Starting position
 * @param inventory           Initial inventory (moved)
 */
Player::Player(int baseStaminaMax, int baseRationRegen, int baseFov,
               int baseRationCapacity, int basePickaxeCapacity, Vector2D pos,
               std::vector<std::unique_ptr<Item> > &&inventory) {
    this->baseStaminaMax = baseStaminaMax;
    this->baseRationRegen = baseRationRegen;
    this->baseFov = baseFov;
    this->baseRationCapacity = baseRationCapacity;
    this->basePickaxeCapacity = basePickaxeCapacity;
    this->pos = pos;
    this->inventory = std::move(inventory);
}

/*
 * Resets player stats to default base stats, multipliers,
 * and starting position (0, 0).
 *
 * Usage:
 * player.resetStats();
 *
 * @return void
 */
void Player::resetStats() {
    this->baseStaminaMax = DEFAULT_STAMINA_MAX;
    this->baseRationRegen = DEFAULT_RATION_REGEN;
    this->baseFov = DEFAULT_FOV;
    this->baseRationCapacity = DEFAULT_RATION_CAPACITY;
    this->basePickaxeCapacity = DEFAULT_PICKAXE_CAPACITY;

    this->stamina = DEFAULT_STAMINA_MAX;
    this->staminaMax = DEFAULT_STAMINA_MAX;
    this->rationRegen = DEFAULT_RATION_REGEN;
    this->fov = DEFAULT_FOV;
    this->rationCapacity = DEFAULT_RATION_CAPACITY;
    this->rationsOwned = 0;
    this->pickaxeCapacity = DEFAULT_PICKAXE_CAPACITY;
    this->pickaxesOwned = 0;

    this->staminaMaxMult = DEFAULT_STAMINA_MAX_MULT;
    this->rationRegenMult = DEFAULT_RATION_REGEN_MULT;
    this->fovMult = DEFAULT_FOV_MULT;
    this->rationCapacityMult = DEFAULT_RATION_CAPACITY_MULT;
    this->pickaxeCapacityMult = DEFAULT_PICKAXE_CAPACITY_MULT;

    this->prevPos = Vector2D(0, 0);
    this->pos = Vector2D(0, 0);
}

/*
 * Adds an item to the player's inventory
 * Transfers ownership of the given item to the inventory.
 *
 * Usage:
 * player.addItem(std::make_unique<Item>());
 *
 * @param item Item to add (unique pointer)
 */
void Player::addItem(std::unique_ptr<Item> &item,
                     std::vector<std::unique_ptr<Item> > &itemList) {
    if (this->inventory.size() >= 5)
        return;

    this->inventory.push_back(std::move(item));
    itemList.erase(std::remove(itemList.begin(), itemList.end(), nullptr),
                   itemList.end());
}

/*
 * Removes an item from the player's inventory
 *
 * Transfers ownership of the given item to the unobtained items list
 *
 * Usage:
 * player.addItem(std::make_unique<Item>());
 *
 * @param item Item to add (unique pointer)
 * @return void
 */
void Player::removeItem(
    int itemID,
    std::vector<std::vector<std::unique_ptr<Item> > > &unobtainedItems) {
    // Doesn't have the item to be removed
    if (!this->hasItem(itemID))
        return;

    // Go through inventory to find the item & move it back to the itemList
    // responsible to hold it
    for (auto &item : this->inventory) {
        if (item->id == itemID) {
            unobtainedItems[item->rarity].push_back(std::move(item));
        }
    }
    this->inventory.erase(
        std::remove(this->inventory.begin(), this->inventory.end(), nullptr),
        this->inventory.end());
}

/*
 * Checks if the player has a specific item
 * Iterates through the inventory to see if an item with the given ID exists.
 *
 * Usage:
 * bool has = player.hasItem(5);
 *
 * @param itemID ID of the item to look for
 * @return true if the item exists in the inventory, false otherwise
 */
bool Player::hasItem(int itemID) const {
    for (auto &item : this->inventory)
        if (item->id == itemID)
            return true;
    return false;
}

/*
 * Resets player stats before applying bonuses
 *
 * Sets all stats and multipliers back to base values,
 * preparing for an update pass.
 *
 * Usage:
 * player.preUpdate();
 *
 * @return void
 */
void Player::preUpdate() {
    
    this->staminaMax = this->baseStaminaMax;
    this->rationRegen = this->baseRationRegen;
    this->fov = this->baseFov;
    this->rationCapacity = this->baseRationCapacity;
    this->pickaxeCapacity = this->basePickaxeCapacity;

    this->staminaMaxMult = 1.0f;
    this->rationRegenMult = 1.0f;
    this->fovMult = 1.0f;
    this->rationCapacityMult = 1.0f;
    this->pickaxeCapacityMult = 1.0f;

}

/*
 * Applies item effects to player stats
 *
 * Loops through inventory, applies flat bonuses and multipliers,
 * and recalculates all final stat values.
 *
 * Usage:
 * player.update();
 */
void Player::update() {

    this->staminaMax = this->baseStaminaMax;
    this->rationRegen = this->baseRationRegen;
    this->fov = this->baseFov;
    this->rationCapacity = this->baseRationCapacity;
    this->pickaxeCapacity = this->basePickaxeCapacity;

    this->staminaMaxMult = 1.0f;
    this->rationRegenMult = 1.0f;
    this->fovMult = 1.0f;
    this->rationCapacityMult = 1.0f;
    this->pickaxeCapacityMult = 1.0f;

    // Snap multipliers to zero if they became NaN or negative
    if (this->staminaMaxMult < 0.0f)
        this->staminaMaxMult = 1.0f;
    if (this->rationRegenMult < 0.0f)
        this->rationRegenMult = 1.0f;
    if (this->fovMult < 0.0f)
        this->fovMult = 1.0f;
    if (this->rationCapacityMult < 0.0f)
        this->rationCapacityMult = 1.0f;
    if (this->pickaxeCapacityMult < 0.0f)
        this->pickaxeCapacityMult = 1.0f;

    for (const auto &item : inventory) {
        if (!item)
            continue;

        if (item->hasCustomBehavior) {
            item->update(*this);
        }

        // Flat bonuses
        this->staminaMax += item->bonusStaminaMax;
        this->rationRegen += item->bonusRationRegen;
        this->fov += item->bonusFov;
        this->rationCapacity += item->bonusRationCapacity;
        this->pickaxeCapacity += item->bonusPickaxeCapacity;

        // Multipliers
        this->staminaMaxMult += item->bonusStaminaMaxMult;
        this->rationRegenMult += item->bonusRationRegenMult;
        this->fovMult += item->bonusFovMult;
        this->rationCapacityMult += item->bonusRationCapacityMult;
        this->pickaxeCapacityMult += item->bonusPickaxeCapacityMult;
    }

    this->staminaMax =
        static_cast<int>(this->staminaMax * this->staminaMaxMult);
    this->rationRegen =
        static_cast<int>(this->rationRegen * this->rationRegenMult);
    this->fov = static_cast<int>(this->fov * this->fovMult);
    this->rationCapacity =
        static_cast<int>(this->rationCapacity * this->rationCapacityMult);
    this->pickaxeCapacity =
        static_cast<int>(this->pickaxeCapacity * this->pickaxeCapacityMult);
}

/*
 * Post-update stat validation
 * Ensures values like stamina and capacity remain in valid ranges.
 * Resets negative multipliers back to 1.0f.
 *
 * Usage:
 * player.postUpdate();
 *
 * @return void
 */
void Player::postUpdate() {
    // Ensure values are within possible range
    if (this->stamina > this->staminaMax)
        this->stamina = this->staminaMax;
    if (this->stamina < 0)
        this->stamina = 0;
    if (this->staminaMax < 1)
        this->staminaMax = 1;
    if (this->rationsOwned < 0)
        this->rationsOwned = 0;
    if (this->rationsOwned > this->rationCapacity)
        this->rationsOwned = this->rationCapacity;
    if (this->pickaxesOwned < 0)
        this->pickaxesOwned = 0;
    if (this->pickaxesOwned > this->pickaxeCapacity)
        this->pickaxesOwned = this->pickaxeCapacity;
    if (this->fov < 1)
        this->fov = 1;
    if (this->fov > 10)
        this->fov = 10;

    this->prevPos = this->pos;
}
