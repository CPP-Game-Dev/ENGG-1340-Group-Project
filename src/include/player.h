#pragma once

#include "vector2d.h"

/*
 * Class to store player-related functions & player data
 */
class Player {
  private:

    int stamina;              // Current stamina
    int staminaMax;           // Maximum stamina
    int fov;                  // Radius of field-of-view
    Vector2D pos;             // Player position

    int rationStaminaBonus;   // Bonus stamina when using rations
    int staminaMaxBonus;      // Bonus max stamina
    int fovBonus;             // Bonus field-of-view
    int rationCapacityBonus;  // Bonus ration carry capacity
    int pickaxeCapacityBonus; // Bonus pickaxe carry capacity

  public:
    Player(); // Initialize a new Player object (used for new game)
    Player(Vector2D pos, int stamina, int staminaMax,
           int fov); // Create a Player object with predetermined stats (used to
                     // load a save)

    #pragma region Getters

    #pragma endregion
    #pragma region Setters

    #pragma endregion
};
