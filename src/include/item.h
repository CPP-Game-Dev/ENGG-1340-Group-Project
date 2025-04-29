#pragma once

#include <vector>
#include <string>
#include <memory>
#include "enums.h"

class Player;

class Item {
public:
    virtual ~Item();

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

    Item(ItemID id);
    Item(ItemID id, const std::string& name, const std::string& description, int property);

    virtual void update(Player& player);
};
