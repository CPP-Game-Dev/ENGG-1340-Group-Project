#pragma once

#include <vector>
#include <string>
#include <memory>
#include "enums.h"

class Player;


class Item {
  public:
    ItemID id;
    std::string name;
    std::string description;
    int rarity;
    int counter;

    bool hasCustomBehavior = false;

    int bonusStaminaMax;
    int bonusRationRegen;
    int bonusFov;
    int bonusRationCapacity;
    int bonusPickaxeCapacity;
    
    float bonusStaminaMaxMult;
    float bonusRationRegenMult;
    float bonusFovMult;
    float bonusRationCapacityMult;
    float bonusPickaxeCapacityMult;

  //constructors
  // default constructor
  //setting all the bonuses and counters to 0.
    Item(ItemID id);
//overloaded constructor:
    Item(ItemID id, const std::string& name, const std::string& description, int property);
//initialises an item with specific values.
//used in loading items data 
//implementation
    void update(Player& player);
  
};

