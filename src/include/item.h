#pragma once

#include <vector>
#include <string>
#include <memory>
#include "enums.h"

class Item {
  public:
    ItemID id;
    std::string name;
    std::string description;
    int rarity;
    int counter;

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
    
    Item(ItemID id);
    void setDefaults();
};

std::unique_ptr<Item> createItem(ItemID itemID);