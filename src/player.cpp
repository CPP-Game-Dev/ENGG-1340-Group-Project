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
int Player::getBaseStamina() const { return baseStamina; }
int Player::getBaseStaminaMax() const { return baseStaminaMax; }
int Player::getBaseRationRegen() const { return baseRationRegen; }
int Player::getBaseFov() const { return baseFov; }
int Player::getBaseRationCapacity() const { return baseRationCapacity; }
int Player::getBasePickaxeCapacity() const { return basePickaxeCapacity; }

float Player::getStaminaMultiplier() const { return staminaMultiplier; }
float Player::getRationRegenMultiplier() const { return rationRegenMultiplier; }
float Player::getFovMultiplier() const { return fovMultiplier; }
float Player::getRationCapacityMultiplier() const { return rationCapacityMultiplier; }
float Player::getPickaxeCapacityMultiplier() const { return pickaxeCapacityMultiplier; }

#pragma endregion
#pragma region Setters
//TODO(Jenna): Implement setters according to their declarations in player.h

void Player::setBaseStamina(int value) { baseStamina = value; }
void Player::setBaseStaminaMax(int value) { baseStaminaMax = value; }
void Player::setBaseRationRegen(int value) { baseRationRegen = value; }
void Player::setBaseFov(int value) { baseFov = value; }
void Player::setBaseRationCapacity(int value) { baseRationCapacity = value; }
void Player::setBasePickaxeCapacity(int value) { basePickaxeCapacity = value; }

void Player::setStaminaMultiplier(float value) { staminaMultiplier = value; }
void Player::setRationRegenMultiplier(float value) { rationRegenMultiplier = value; }
void Player::setFovMultiplier(float value) { fovMultiplier = value; }
void Player::setRationCapacityMultiplier(float value) { rationCapacityMultiplier = value; }
void Player::setPickaxeCapacityMultiplier(float value) { pickaxeCapacityMultiplier = value; }

void Player::setPosition(const Vector2D& pos) { position = pos; }

#pragma endregion
