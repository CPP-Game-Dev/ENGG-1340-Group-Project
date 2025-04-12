#include "player.h"
#include "include/iitem.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <iostream>
#include <memory>
#include <vector>

Player::Player() {
    /*
    * TODO(Jenna): Implement default constructor to set default values for class variables
    * Ensure that all multipliers are set to 1
    * All base variables are set to 0
    * All other variables are set to 0 as well
    * Pos is set to 0, 0
    * Ignore inventory, let me(Arthur) deal with it
    */ 
    this->baseStamina = 0;
    this->baseStaminaMax = 0;
    this->baseRationRegen = 0;
    this->baseFov = 0;
    this->baseRationCapacity = 0;
    this->basePickaxeCapacity = 0;
  
    this->staminaMultiplier = 1;
    this->rationRegenMultiplier = 1;
    this->fovMultiplier = 1;
    this->rationCapacityMultiplier = 1;
    this->pickaxeCapacityMultiplier = 1;
  
    this->position = Vector2D(0, 0);
}

Player::Player( int baseStamina, int baseStaminaMax, int baseRationRegen,
                int baseFov, int baseRationCapacity, int basePickaxeCapacity,
                Vector2D pos, std::vector<std::unique_ptr<IItem>> inventory)
    : baseStamina(baseStamina), baseStaminaMax(baseStaminaMax),
      baseRationRegen(baseRationRegen), baseFov(baseFov),
      baseRationCapacity(baseRationCapacity), basePickaxeCapacity(basePickaxeCapacity),
      position(pos), inventory(std::move(inventory)) {
    /*
    * TODO(Jenna): Implement parametric constructor 
    * Set each class variable to the input value
    * Do not touch any class variable that doesn't have an input
    */
}

#pragma region Getters
//TODO(Jenna): Implement getters according to their declarations in player.h 
int Player::getBaseStamina() const { return this-> baseStamina; }
int Player::getBaseStaminaMax() const { return this-> baseStaminaMax; }
int Player::getBaseRationRegen() const { return this-> baseRationRegen; }
int Player::getBaseFov() const { return this-> baseFov; }
int Player::getBaseRationCapacity() const { return this->baseRationCapacity; }
int Player::getBasePickaxeCapacity() const { return this-> basePickaxeCapacity; }

float Player::getStaminaMultiplier() const { return this->staminaMultiplier; }
float Player::getRationRegenMultiplier() const { return this->rationRegenMultiplier; }
float Player::getFovMultiplier() const { return this->fovMultiplier; }
float Player::getRationCapacityMultiplier() const { return this-> rationCapacityMultiplier; }
float Player::getPickaxeCapacityMultiplier() const { return this-> pickaxeCapacityMultiplier; }
#pragma endregion
#pragma region Setters
//TODO(Jenna): Implement setters according to their declarations in player.h

void Player::setBaseStamina(int value) { this->baseStamina = value; }
void Player::setBaseStaminaMax(int value) { this->baseStaminaMax = value; }
void Player::setBaseRationRegen(int value) { this->baseRationRegen = value; }
void Player::setBaseFov(int value) { this->baseFov = value; }
void Player::setBaseRationCapacity(int value) { this->baseRationCapacity = value; }
void Player::setBasePickaxeCapacity(int value) { this->basePickaxeCapacity = value; }

void Player::setStaminaMultiplier(float value) { this->staminaMultiplier = value; }
void Player::setRationRegenMultiplier(float value) { this->rationRegenMultiplier = value; }
void Player::setFovMultiplier(float value) { this->fovMultiplier = value; }
void Player::setRationCapacityMultiplier(float value) { this->rationCapacityMultiplier = value; }
void Player::setPickaxeCapacityMultiplier(float value) { this->pickaxeCapacityMultiplier = value; }

void Player::setPosition(const Vector2D& pos) { this->position = pos; }

=======
     * TODO(Jenna): Implement default constructor to set default values for
     * class variables Ensure that all multipliers are set to 1 All base
     * variables are set to 0 All other variables are set to 0 as well Pos is
     * set to 0, 0 Ignore inventory, let me(Arthur) deal with it
     */
}

Player::Player(int baseStamina, int baseStaminaMax, int baseRationRegen,
               int baseFov, int baseRationCapacity, int basePickaxeCapacity,
               Vector2D pos, std::vector<std::unique_ptr<IItem> > inventory) {
    /*
     * TODO(Jenna): Implement parametric constructor
     * Set each class variable to the input value
     * Do not touch any class variable that doesn't have an input
     */
}

void Player::addItem(std::unique_ptr<IItem> item) {
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
}
void Player::postUpdate() {
    // TODO(Arthur): come up with something to put here or delete the whole
    // function
}

#pragma region Getters
// TODO(Jenna): Implement getters according to their declarations in player.h
#pragma endregion
#pragma region Setters
// TODO(Jenna): Implement setters according to their declarations in player.h
#pragma endregion
