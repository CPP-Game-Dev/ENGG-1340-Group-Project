#pragma once

#include "item.h"
#include "vector2d.h"
#include <memory>
#include <vector>

const int DEFAULT_STAMINA = 100;
const int DEFAULT_STAMINA_MULT = 100;
const int DEFAULT_RATION_REGEN = 0;
const int DEFAULT_RATION_REGEN_MULT = 0;
const int DEFAULT_FOV = 3;
const int DEFAULT_FOV_MULT = 1;
const int DEFAULT_RATION_CAPACITY = 0;
const int DEFAULT_PICKAXE_CAPACITY = 100;
const int DEFAULT_RATION_CAPACITY_MULT = 1;
const int DEFAULT_PICKAXE_CAPACITY_MULT = 1;

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
#pragma region Current Stats
    int stamina;         // Current stamina
    int staminaMax;      // Current max stamina
    int rationRegen;     // Current ration regeneration amount
    int fov;             // Current FOV
    int rationCapacity;  // Current ration capacity
    int pickaxeCapacity; // Current pickaxe capacity
    Vector2D prevPos;    // Previous position
    Vector2D pos;        // Current position
#pragma region endregion
#pragma region Stat Multipliers
    float staminaMaxMult;      // Multiplier for max stamina
    float rationRegenMult;     // Multiplier for ration stamina regen
    float fovMult;             // Multiplier for FOV
    float rationCapacityMult;  // Multiplier for ration capacity
    float pickaxeCapacityMult; // Multiplier for pickaxe capacity
#pragma endregion
    std::vector<std::unique_ptr<Item> > inventory; // Inventory storage

  public:
    Player(); // Initialize a new Player object (used for new game)
    Player(int baseStaminaMax, int baseRationRegen, int baseFov,
           int baseRationCapacity, int basePickaxeCapacity, Vector2D pos,
           std::vector<std::unique_ptr<Item> > &&inventory);

#pragma region Inventory Management

    template <typename ItemType>
    void removeItem(std::vector<std::unique_ptr<Item> > itemList) {
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

void addItem(std::unique_ptr<Item> item);

const std::vector<std::unique_ptr<Item>>& getInventory() const;


template <typename ItemType>
void removeItemTo(std::vector<std::unique_ptr<Item>>& itemList) {
  
  auto it = inventory.begin();
  
        while (it != inventory.end()) {
          
            if (dynamic_cast<ItemType*>(it->get()) != nullptr) {
              
                itemList.push_back(std::move(*it));
                it = inventory.erase(it);
            } 
            else {
                ++it;
            }
        }
    }

  
#pragma endregion

    void preUpdate();  // Prepares the player instance for update()
    void update();     // Updates statz & shitz
    void postUpdate(); // Cleans up

#pragma region Getters
    inline int getBaseStaminaMax() const { return this->baseStaminaMax; }
    inline int getBaseRationRegen() const { return this->baseRationRegen; }
    inline int getBaseFov() const { return this->baseFov; }
    inline int getBaseRationCapacity() const {
        return this->baseRationCapacity;
    }
    inline int getBasePickaxeCapacity() const {
        return this->basePickaxeCapacity;
    }

    inline int getStamina() const { return this->stamina; }
    inline int getStaminaMax() const { return this->staminaMax; }
    inline int getRationRegen() const { return this->rationRegen; }
    inline int getFov() const { return this->fov; }
    inline int getRationCapacity() const { return this->rationCapacity; }
    inline int getPickaxeCapacity() const { return this->pickaxeCapacity; }

    inline Vector2D getPos() const { return this->pos; }

    inline double getStaminaMaxMult() const { return this->staminaMaxMult; }
    inline double getRationRegenMult() const { return this->rationRegenMult; }
    inline double getFovMult() const { return this->fovMult; }
    inline double getRationCapacityMult() const {
        return this->rationCapacityMult;
    }
    inline double getPickaxeCapacityMult() const {
        return this->pickaxeCapacityMult;
    }
#pragma endregion
#pragma region Setters
    inline void setBaseStaminaMax(int value) { this->baseStaminaMax = value; }
    inline void setBaseRationRegen(int value) { this->baseRationRegen = value; }
    inline void setBaseFov(int value) { this->baseFov = value; }
    inline void setBaseRationCapacity(int value) {
        this->baseRationCapacity = value;
    }
    inline void setBasePickaxeCapacity(int value) {
        this->basePickaxeCapacity = value;
    }

    inline void setStamina(int value) { this->stamina = value; }
    inline void setStaminaMax(int value) { this->staminaMax = value; }
    inline void setRationRegen(int value) { this->rationRegen = value; }
    inline void setFov(int value) { this->fov; }
    inline void setRationCapacity(int value) { this->rationCapacity = value; }
    inline void setPickaxeCapacity(int value) { this->pickaxeCapacity = value; }

    inline void setPos(int y, int x) { this->pos = Vector2D(y, x); }
    inline void setPos(Vector2D newPos) { this->pos = newPos; }

    inline void setStaminaMaxMult(float value) { this->staminaMaxMult = value; }
    inline void setRationRegenMult(float value) {
        this->rationRegenMult = value;
    }
    inline void setFovMult(float value) { this->fovMult = value; }
    inline void setRationCapacityMult(float value) {
        this->rationCapacityMult = value;
    }
    inline void setPickaxeCapacityMult(float value) {
        this->pickaxeCapacityMult = value;
    }
#pragma endregion
};
