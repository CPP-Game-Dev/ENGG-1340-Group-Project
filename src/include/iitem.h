#pragma once

#include "player.h"

class IItem
{
  protected:
    int *counter;           // Store counters for custom item behavior
    char *sprite;           // Stores the megapixel sprite of the item
    int rarity;               // Rarity for item generation weights, 0 is rarest [0-4] 
  public:
    virtual void update(Player player);
    virtual ~IItem();
};