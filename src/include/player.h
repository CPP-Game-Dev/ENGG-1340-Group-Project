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
    Vector2D getPos() const;
    int getStamina() const;
    int getStaminaMax() const;
    int getFov() const;
    int getRationStaminaBonus() const;
    int getStaminaMaxBonus() const;
    int getFovBonus() const;
    int getRationCapacityBonus() const;
    int getPickaxeCapacityBonus() const;
    #pragma endregion
    #pragma region Setters
    void setPos(const Vector2D pos);
    void setStamina(const int value);
    void setStaminaMax(const int value);
    void setFov(const int value);
    void setRationStaminaBonus(const int value);
    void setStaminaMaxBonus(const int value);
    void setFovBonus(const int value);
    void setRationCapacityBonus(const int value);
    void setPickaxeCapacityBonus(const int value);
    #pragma endregion
};
