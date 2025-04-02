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

    
}

Player::Player( int baseStamina, int baseStaminaMax, int baseRationRegen,
                int baseFov, int baseRationCapacity, int basePickaxeCapacity,
                Vector2D pos, std::vector<std::unique_ptr<IItem>> inventory) {
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
    * TODO(James): loop through player inventory and call update() of each item (if hasCustomBehavior == true)
    *              Add the flat stat bonuses to the player's current stats respectively
    *              Add the stat mults to the player's respective mults
    *              after all that, multiply each of the player's current stat with their respective mult
    */ 
}
void Player::postUpdate() {
    // TODO(Arthur): come up with something to put here or delete the whole function
}

#pragma region Getters
//TODO(Jenna): Implement getters according to their declarations in player.h 
#pragma endregion
#pragma region Setters
//TODO(Jenna): Implement setters according to their declarations in player.h
#pragma endregion
