#pragma once

#include <vector>
#include <string>
#include "enums.h"

class IItem
{
  protected:
    std::string sprite;       // Stores the megapixel sprite of the item
    Rarity rarity;            // Rarity for item generation weights, 0 is rarest [0-4] 
    std::string displayName;  // Name of the item displayed
    std::string description;  // Description of the item
    std::vector<int> counter; // Store counters for custom item behavior
    bool hasCustomBehavior;   // Bool to tell holder of the item if it has custom behavior
    /*
    * Note by Arthur: We may need to use a json locization system for the names & desc
    * If they get too long that is
    */

    #pragma region Flat Stat Bonuses
    int staminaMaxBonus;      // Flat bonus for max stamina
    int rationRegenBonus;     // Flat bonus for ration stamina regen
    int fovBonus;             // Flat bonus for FOV
    int rationCapacityBonus;  // Flat bonus for ration capacity
    int pickaxeCapacityBonus; // Flat bonus for pickaxe capacity
    #pragma endregion

    #pragma region Stat Multipliers
    double staminaMaxMult;       // Multiplier for max stamina
    double rationRegenMult;      // Multiplier for ration stamina regen
    double fovMult;              // Multiplier for FOV
    double rationCapacityMult;   // Multiplier for ration capacity
    double pickaxeCapacityMult;  // Multiplier for pickaxe capacity
    #pragma endregion
    

  public:
    IItem() { 
        setDefaults(); 
    }
    virtual ~IItem() = default; 

    /*
    * Function intended to be overridden & called from derived classes
    * Sets all class variables to a default value 
    * Inherited class can call this and only change whatever they want
    */
    inline void setDefaults()
    {
        // Set 
        this->sprite = "  ";
        this->rarity = Rarity::Common;
        this->displayName = "itemName";
        this->description = "itemDescription";
        this->counter.reserve(1);
        this->hasCustomBehavior = false;  

        // Reset flat bonuses
        this->staminaMaxBonus = 0;
        this->rationRegenBonus = 0;
        this->fovBonus = 0;
        this->rationCapacityBonus = 0;
        this->pickaxeCapacityBonus = 0;
        
        // Reset mults
        this->staminaMaxMult = 0;
        this->rationRegenMult = 0;
        this->fovMult = 0;
        this->rationCapacityMult = 0;
        this->pickaxeCapacityMult = 0;
    }
};