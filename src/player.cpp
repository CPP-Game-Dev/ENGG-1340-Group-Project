#include "include/player.h"
#include "include/item.h"
#include "include/vector2d.h"
// #include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

/*
 * Constructor for default player
 *
 * Initializes player with default base stats, multipliers,
 * and starting position (0, 0).
 *
 * Usage:
 * Player player;
 *
 * @return none
 */
Player::Player() {
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
 * Constructor with custom base stats and inventory
 *
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
 * @return none
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
 * Adds an item to the player's inventory
 *
 * Transfers ownership of the given item to the inventory.
 *
 * Usage:
 * player.addItem(std::make_unique<Item>());
 *
 * @param item Item to add (unique pointer)
 * @return void
 */
void Player::addItem(std::unique_ptr<Item> &item,
                     std::vector<std::unique_ptr<Item> > &itemList) {
    if (this->inventory.size() >= 5)
        return;

    this->inventory.push_back(std::move(item));
    itemList.erase(std::remove(itemList.begin(), itemList.end(), nullptr),
                   itemList.end());
}

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

bool Player::hasItem(int itemID) {
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

    // Prepares the player stats for update by resetting them to their base
    // values. Also resets all stat multipliers to their default value (1.0).
    /*
     * TODO(Jenna): set all current stats to their corrosponding base stats
     *              (don't touch the ones without a base stat)
     *              And set all mults to 1
     */

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

    // Note: Stamina itself is NOT reset here; it depends on gameplay logic
    // elsewhere.
}

/*
 * Applies item effects to player stats
 *
 * Loops through inventory, applies flat bonuses and multipliers,
 * and recalculates all final stat values.
 *
 * Usage:
 * player.update();
 *
 * @return void
 */
void Player::update() {
    /*
     * TODO(James): loop through player inventory and call update() of each item
     * (if hasCustomBehavior == true) Add the flat stat bonuses to the player's
     * current stats respectively Add the stat mults to the player's respective
     * mults after all that, multiply each of the player's current stat with
     * their respective mult
     */
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
 *
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

    // Optional: Snap multipliers to zero if they became NaN or negative
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

    this->prevPos = this->pos;
}
