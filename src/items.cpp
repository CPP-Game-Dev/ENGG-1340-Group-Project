#include "include/iitem.h"
#include "include/items.h"
#include "include/player.h"
#include <vector>

#pragma region ExampleItem
ExampleItem::ExampleItem() {
    this->rarity = 0;           // Set rarity as appropriate
    this->sprite = "__";        // Set sprite as approptiate
    this->counter.reserve(2);   // Set the number of counters you need
    this->hasTriggered = false;
    this->displayName = "Example Item";
    this->description = "I am an example item, I serve no gameplay purpose";
}

void ExampleItem::update(Player player) {
    /*
    * Update player stats & stuff during inventory update
    * i.e. increasing the player's stats, performing certain actions
    */

    player.staminaMax += 3;

    /*
    * For more complex logic, you'll need to use the counter
    * Suppose you want to give a ration for each 5 steps taken
    * Increment the counter each time the plater is displaced
    * When counter[0] >= 5, give a ration(if they can hold more)
    * and reset counter
    */

    // TODO(Arthur): Implement example

}

// Lastly, don't forget to encapsulate everything with #pragma region ItemName & #pragma endregion
#pragma endregion