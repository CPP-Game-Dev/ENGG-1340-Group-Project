#include "include/iitem.h"
#include "include/items.h"
#include "include/enums.h"
#include "include/player.h"
#include <vector>

#pragma region ExampleItem

void ExampleItem::setDefaults() {
    IItem::setDefaults();
    this->rarity = Rarity::Common;         // Set rarity as appropriate
    this->sprite = "__";      // Set sprite as approptiate
    this->counter.reserve(2); // Set the number of counters you need
    this->hasTriggered = false;
    this->displayName = "Example Item";
    this->description = "I am an example item, I serve no gameplay purpose";
    this->staminaMaxBonus = 3;
}

void ExampleItem::update(Player player) {
    // TODO(Arthur): Implement example of custom behavior
}

// Lastly, don't forget to encapsulate everything with #pragma region ItemName & #pragma endregion
#pragma endregion