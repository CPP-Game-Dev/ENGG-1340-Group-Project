#include "include/player.h"
#include "include/item.h"
#include "include/vector2d.h"
// #include <iostream>
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
    this->baseStaminaMax = DEFAULT_STAMINA;
    this->baseRationRegen = DEFAULT_RATION_REGEN;
    this->baseFov = DEFAULT_FOV;
    this->baseRationCapacity = DEFAULT_RATION_CAPACITY;
    this->basePickaxeCapacity = DEFAULT_PICKAXE_CAPACITY;

    this->stamina = DEFAULT_STAMINA;
    this->staminaMaxMult = DEFAULT_STAMINA_MULT;
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
void Player::addItem(std::unique_ptr<Item> item) {
    this->inventory.push_back(std::move(item));
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
    

    // Prepares the player stats for update by resetting them to their base values.
    // Also resets all stat multipliers to their default value (1.0).
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

 // Note: Stamina itself is NOT reset here; it depends on gameplay logic elsewhere.


    
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
    staminaMax         = baseStaminaMax;
    rationRegen        = baseRationRegen;
    fov                = baseFov;
    rationCapacity     = baseRationCapacity;
    pickaxeCapacity    = basePickaxeCapacity;

    staminaMaxMult         = 1.0f;
    rationRegenMult        = 1.0f;
    fovMult                = 1.0f;
    rationCapacityMult     = 1.0f;
    pickaxeCapacityMult    = 1.0f;


    for (const auto& item : inventory) {
        if (!item) continue;

        if (item->hasCustomBehavior) {
            item->update(*this);
        }

        // Flat bonuses
        staminaMax         += item->bonusStaminaMax;
        rationRegen        += item->bonusRationRegen;
        fov                += item->bonusFov;
        rationCapacity     += item->bonusRationCapacity;
        pickaxeCapacity    += item->bonusPickaxeCapacity;

        // Multipliers
        staminaMaxMult         *= item->bonusStaminaMaxMult;
        rationRegenMult        *= item->bonusRationRegenMult;
        fovMult                *= item->bonusFovMult;
        rationCapacityMult     *= item->bonusRationCapacityMult;
        pickaxeCapacityMult    *= item->bonusPickaxeCapacityMult;
    }

    staminaMax         = static_cast<int>(staminaMax * staminaMaxMult);
    rationRegen        = static_cast<int>(rationRegen * rationRegenMult);
    fov                = static_cast<int>(fov * fovMult);
    rationCapacity     = static_cast<int>(rationCapacity * rationCapacityMult);
    pickaxeCapacity    = static_cast<int>(pickaxeCapacity * pickaxeCapacityMult);
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
    if (stamina > staminaMax) {
        stamina = staminaMax;
    }
    if (stamina < 0) {
        stamina = 0;
    }

    if (rationCapacity < 0) {
        rationCapacity = 0;
    }

    if (pickaxeCapacity < 0) {
        pickaxeCapacity = 0;
    }

    if (fov < 1) {
        fov = 1;
    }

    // Optional: Snap multipliers to zero if they became NaN or negative
    if (staminaMaxMult < 0.0f) staminaMaxMult = 1.0f;
    if (rationRegenMult < 0.0f) rationRegenMult = 1.0f;
    if (fovMult < 0.0f) fovMult = 1.0f;
    if (rationCapacityMult < 0.0f) rationCapacityMult = 1.0f;
    if (pickaxeCapacityMult < 0.0f) pickaxeCapacityMult = 1.0f;

    // TODO(Arthur): come up with something to put here or delete the whole
    // function

    
}

// Vector2D Player::getPos() const { return this->pos; }
