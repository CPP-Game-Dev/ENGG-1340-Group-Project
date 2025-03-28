#pragma once

#include "player.h"
#include <vector>
#include <string>

class IItem
{
  protected:
    std::vector<int> counter; // Store counters for custom item behavior
    char *sprite;             // Stores the megapixel sprite of the item
    int rarity;               // Rarity for item generation weights, 0 is rarest [0-4] 
    std::string displayName;  // Name of the item displayed
    std::string description;  // Description of the item
    /*
    * Note by Arthur: We may need to use a json locization system for the names & desc
    * If they get too long that is
    */
  public:
    virtual void update(Player player);
    virtual ~IItem();
};