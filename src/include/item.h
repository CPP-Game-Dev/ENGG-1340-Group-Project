#pragma once

#include <vector>
#include <string>
#include <memory>
#include "enums.h"

class Player;


 /*
 * Represents an item that grants stat bonuses or effects to the player.
 * Items are usually loaded from data and stored in inventory.
 */

class Item {
  public:
    int id;
    std::string name;
    std::string description;
    int rarity;
    int counter;

    bool hasCustomBehavior = false;

    int bonusStaminaMax;
    int bonusRationRegen;
    int bonusFov;
    int bonusRationCapacity;
    int bonusPickaxeCapacity;
    
    float bonusStaminaMaxMult;
    float bonusRationRegenMult;
    float bonusFovMult;
    float bonusRationCapacityMult;
    float bonusPickaxeCapacityMult;

  //constructors
    Item();
    Item(int id, const std::string& name, const std::string& description, int rarity, bool hasCustomBehavior, const std::vector<int> &flatBonus, const std::vector<float> &mult);

    void update(Player& player);
  
};

struct ItemID {
    // Common
    static const int CampingBoots = 1;
    static const int OldBelt = 2;
    static const int HoardersBackpack = 3;
    static const int CharredSteak = 4;
    static const int RottenEgg = 5;
    static const int OilLamp = 6;
    static const int ExpiredMilk = 7;
    static const int WornSneakers = 8;
    static const int SpinachLeaf = 9;
    static const int InkBottle = 10;
    static const int OversizedCookie = 11;
    static const int MutatedPotato = 12;
    static const int BlueCheese = 13;
    static const int SleepingBag = 14;
    // Uncommon
    static const int CampingBackpack = 15;
    static const int CampingFlashlight = 16;
    static const int CharmOfSatiation = 17;
    static const int EmergencyRations = 18;
    static const int RollerSkates = 19;
    static const int LitTorch = 20;
    static const int MRE = 21;
    static const int MillitaryBackpack = 22;
    static const int EnchantedBracelet = 23;
    static const int GTCDV1 = 24;
    static const int GTCDV2 = 25;
    static const int BundleOfBreadsticks = 26;
    // Rare
    static const int Headlights = 27;
    static const int TelescopeGlasses = 28;
    static const int MetalDetector = 29;
    static const int MagicMushroom = 30;
    static const int MiningHelmet = 31;
    static const int SuspiciousPills = 32;
    static const int InsulinInjection = 33;
    static const int Wheelchair = 34;
    static const int HikingStaff = 35;
    static const int PremiumSportswear = 36;
    // Relic
    static const int HermesBoots = 37;
    static const int QuadEspresso = 38;
    static const int SunSeed = 39;
    static const int AStar = 40;
    static const int RedBull = 41;
};

