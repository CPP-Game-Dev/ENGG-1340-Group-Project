#pragma once

#include "iitem.h"
#include "vector2d.h"
#include <memory>
#include <vector>

/*
 * Class to store player-related functions & player data
 */
class Player {
  private:
#pragma region Base Stats
    int baseStaminaMax;  // The max amount of stamina that can be carried at any
                         // time
    int baseRationRegen; // The amount of stamina regenerated when using a
                         // ration
    int baseFov;         // The radius of FOV
    int baseRationCapacity;  // The number of rations that can be carried
    int basePickaxeCapacity; // The number of pickaxes that can be carried
#pragma endregion

    std::vector<std::unique_ptr<IItem> > inventory; // Inventory storage

  public:
#pragma region Current Stats
    int stamina;         // Current stamina
    int staminaMax;      // Current max stamina
    int rationRegen;     // Current ration regeneration amount
    int fov;             // Current FOV
    int rationCapacity;  // Current ration capacity
    int pickaxeCapacity; // Current pickaxe capacity
    Vector2D pos;        // Current position
#pragma region endregion
#pragma region Stat Multipliers
    double staminaMaxMult;      // Multiplier for max stamina
    double rationRegenMult;     // Multiplier for ration stamina regen
    double fovMult;             // Multiplier for FOV
    double rationCapacityMult;  // Multiplier for ration capacity
    double pickaxeCapacityMult; // Multiplier for pickaxe capacity
#pragma endregion

    Player(); // Initialize a new Player object (used for new game)
    Player(int baseStamina, int baseStaminaMax, int baseRationRegen,
           int baseFov, int baseRationCapacity, int basePickaxeCapacity,
           Vector2D pos,
           std::vector<std::unique_ptr<IItem> >
               inventory); // Set player base stats, pos & inventory to given
                           // values



    #pragma region Inventory Management
    void addItem(std::unique_ptr<IItem>);

    template <typename ItemType>
    void removeItem(std::vector<std::unique_ptr<IItem> > itemList) {
        for (auto &item : this->inventory) {
            if (dynamic_cast<ItemType *>(item.get()) != nullptr) {
                itemList.push_back(std::move(item));
            }
        }
    }

    template <typename ItemType> bool hasItem() {
        for (auto &item : this->inventory)
            if (dynamic_cast<ItemType *>(item.get()) != nullptr)
                return true;
        return false;
    }
    #pragma endregion


    void preUpdate();   // Prepares the player instance for update()
    void update();      // Updates statz & shitz
    void postUpdate();  // Cleans up

    #pragma region Getters
    // TOD(Jenna):Declare getters for all base stats
    #pragma endregion
    #pragma region Setters
    // TODO(Jenna):Declare setters for all base stats
    #pragma endregion
};
