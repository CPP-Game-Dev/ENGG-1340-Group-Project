#include "player.h"
#include "include/player.h"
#include "include/vector2d.h"
#include "include/iitem.h"
#include <iostream>
#include <vector>
#include <memory>

Player::Player() {
    /*
    * TODO(Jenna): Implement default constructor to set default values for class variables
    * Ensure that all multipliers are set to 1
    * All base variables are set to 0
    * All other variables are set to 0 as well
    * Pos is set to 0, 0
    * Ignore inventory, let me(Arthur) deal with it
    */ 
    this-> baseStamina = 0;
    this-> baseStaminaMax = 0;
    this-> baseRationRegen = 0;
    this-> baseFov = 0;
    this-> baseRationCapacity = 0;
    this-> basePickaxeCapacity = 0;
    this-> staminaMultiplier = 1;
    this-> rationRegenMultiplier = 1;
    this-> fovMultiplier = 1;
    this-> rationCapacityMultiplier = 1;
    this-> pickaxeCapacityMultiplier = 1;
    this-> position = Vector2D(0, 0);
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

void Player::setBaseStamina(int value) { this-> baseStamina = value; }
void Player::setBaseStaminaMax(int value) { this-> baseStaminaMax = value; }
void Player::setBaseRationRegen(int value) { this-> baseRationRegen = value; }
void Player::setBaseFov(int value) { this-> baseFov = value; }
void Player::setBaseRationCapacity(int value) { this-> baseRationCapacity = value; }
void Player::setBasePickaxeCapacity(int value) { this-> basePickaxeCapacity = value; }

void Player::setStaminaMultiplier(float value) { this-> staminaMultiplier = value; }
void Player::setRationRegenMultiplier(float value) { this-> rationRegenMultiplier = value; }
void Player::setFovMultiplier(float value) { this-> fovMultiplier = value; }
void Player::setRationCapacityMultiplier(float value) { this-> rationCapacityMultiplier = value; }
void Player::setPickaxeCapacityMultiplier(float value) { this-> pickaxeCapacityMultiplier = value; }

void Player::setPosition(const Vector2D& pos) { this-> position = pos; }

#pragma endregion
