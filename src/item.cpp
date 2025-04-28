
#include <string>
#include "include/item.h"
#include "include/enums.h"
#include "include/utils.h"



Item::Item(ItemID id) {
    this->id = id;

    this->name = "Item";
    this->description = "Description";
    this->rarity = 0;
    
    this->counter = 0;

    this->bonusStaminaMax = 0;
    this->bonusRationRegen = 0;
    this->bonusFov = 0;
    this->bonusRationCapacity = 0;
    this->bonusPickaxeCapacity = 0;
    
    this->bonusStaminaMaxMult = 0.0;
    this->bonusRationRegenMult = 0.0;
    this->bonusFovMult = 0.0;
    this->bonusRationCapacityMult = 0.0;
    this->bonusPickaxeCapacityMult = 0.0;
}
Item::Item(ItemID id, const std::string& name, const std::string& description, int property) {
this->id = id;
this->name = name;
this->description = description;
this->rarity = property;

this->counter = 0;

this->bonusStaminaMax = 0;
this->bonusRationRegen = 0;
this->bonusFov = 0;
this->bonusRationCapacity = 0;
this->bonusPickaxeCapacity = 0;
    
this->bonusStaminaMaxMult = 0.0;
this->bonusRationRegenMult = 0.0;
this->bonusFovMult = 0.0;
this->bonusRationCapacityMult = 0.0;
this->bonusPickaxeCapacityMult = 0.0;
