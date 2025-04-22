#include "include/player.h"
#include "include/iitem.h"
#include "include/player.h"
#include "include/vector2d.h"
// #include <iostream>
#include <memory>
#include <vector>

Player::Player() {
    this->baseStaminaMax = 0;
    this->baseRationRegen = 0;
    this->baseFov = 3;
    this->baseRationCapacity = 0;
    this->basePickaxeCapacity = 0;

    this->staminaMaxMult = 1;
    this->rationRegenMult = 1;
    this->fovMult = 1;
    this->rationCapacityMult = 1;
    this->pickaxeCapacityMult = 1;

    this->pos = Vector2D(0, 0);
}

Player::Player(int baseStaminaMax, int baseRationRegen, int baseFov,
               int baseRationCapacity, int basePickaxeCapacity, Vector2D pos,
               std::vector<std::unique_ptr<IItem> > &&inventory) {
    this->baseStaminaMax = baseStaminaMax;
    this->baseRationRegen = baseRationRegen;
    this->baseFov = baseFov;
    this->baseRationCapacity = baseRationCapacity;
    this->basePickaxeCapacity = basePickaxeCapacity;
    this->pos = pos;
    this->inventory = std::move(inventory);
}

void Player::addItem(std::unique_ptr<IItem> &item) {
    this->inventory.push_back(std::move(item));
}

void Player::preUpdate() {
    /*
     * TODO(Jenna): set all current stats to their corrosponding base stats
     *              (don't touch the ones without a base stat)
     *              And set all mults to 1
     */
}
void Player::update() {
    /*
     * TODO(James): loop through player inventory and call update() of each item
     * (if hasCustomBehavior == true) Add the flat stat bonuses to the player's
     * current stats respectively Add the stat mults to the player's respective
     * mults after all that, multiply each of the player's current stat with
     * their respective mult
     */
    this->fov *= this->fovMult;
    this->staminaMax *= this->staminaMaxMult;
}
void Player::postUpdate() {
    // TODO(Arthur): come up with something to put here or delete the whole
    // function
}

// Vector2D Player::getPos() const { return this->pos; }
