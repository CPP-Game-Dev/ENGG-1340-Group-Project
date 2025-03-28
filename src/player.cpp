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

#pragma region Getters
//TODO(Jenna): Implement getters according to their declarations in player.h 
#pragma endregion
#pragma region Setters
//TODO(Jenna): Implement setters according to their declarations in player.h
#pragma endregion
