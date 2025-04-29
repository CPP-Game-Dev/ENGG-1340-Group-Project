#include "include/player.h"
#include "include/item.h"
#include "include/vector2d.h"
// #include <iostream>
#include <memory>
#include <vector>

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

void Player::addItem(std::unique_ptr<Item> item) {
    this->inventory.push_back(std::move(item));
}

void Player::preUpdate() {
    

    // Prepares the player stats for update by resetting them to their base values.
    // Also resets all stat multipliers to their default value (1.0).
    /*
     * set all current stats to their corrosponding base stats
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
     * TODO(James): loop through player inventory and call update() of each item
     * (if hasCustomBehavior == true) Add the flat stat bonuses to the player's
     * current stats respectively Add the stat mults to the player's respective
     * mults after all that, multiply each of the player's current stat with
     * their respective mult
     */
void Player::update() {
    for (auto& item : inventory) {
        if (item->hasCustomBehavior) {
            item->update(*this);
        }

        staminaMax        += item->bonusStaminaMax;
        rationRegen       += item->bonusRationRegen;
        fov               += item->bonusFov;
        rationCapacity    += item->bonusRationCapacity;
        pickaxeCapacity   += item->bonusPickaxeCapacity;

        
        staminaMaxMult      *= item->bonusStaminaMaxMult;
        rationRegenMult     *= item->bonusRationRegenMult;
        fovMult             *= item->bonusFovMult;
        rationCapacityMult  *= item->bonusRationCapacityMult;
        pickaxeCapacityMult *= item->bonusPickaxeCapacityMult;
    }


    staminaMax        = static_cast<int>(staminaMax * staminaMaxMult);
    rationRegen       = static_cast<int>(rationRegen * rationRegenMult);
    fov               = static_cast<int>(fov * fovMult);
    rationCapacity    = static_cast<int>(rationCapacity * rationCapacityMult);
    pickaxeCapacity   = static_cast<int>(pickaxeCapacity * pickaxeCapacityMult);
}


void Player::postUpdate() {
    // TODO(Arthur): come up with something to put here or delete the whole
    // function
}

// Vector2D Player::getPos() const { return this->pos; }
