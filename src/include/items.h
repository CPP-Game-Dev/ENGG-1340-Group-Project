#pragma once

#include "iitem.h"
#include "player.h"

/*
* This file contains all class declarations for the in game items
* Note that each item has it's own class
*/

// This class serves as an example & framework as to how items should be implemented
class ExampleItem : public IItem
{
  public:
    ExampleItem();
    ~ExampleItem();

    void update(Player player) override;
};