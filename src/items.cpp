#include "include/iitem.h"
#include "include/items.h"
#include "include/player.h"

#pragma region ExampleItem
ExampleItem::ExampleItem() {
    this->rarity = 0;           // Set rarity as appropriate
    this->sprite = "__";        // Set sprite as approptiate
    this->counter = new int[2]; // Allocate memory for as many counters as you need
}

ExampleItem::~ExampleItem() { delete this->counter; }      // Don't forget to free the allocated memory

void ExampleItem::update(Player player) {
    /*
    * Update player stats & stuff during inventory update
    * i.e. increasing the player's stats, performing certain actions
    */

    /*
    * For more complex logic, you'll need to use the counter
    * Suppose you want to give a ration for each 5 steps taken
    * Increment the counter each time the plater is displaced
    * When counter[0] >= 5, give a ration(if they can hold more)
    * and reset counter
    */
}

// Lastly, don't forget to encapsulate everything with #pragma region ItemName & #pragma endregion
#pragma endregion