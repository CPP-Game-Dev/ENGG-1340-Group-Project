
#include "include/item.h"
#include "include/enums.h"
#include "include/player.h"

#include <string>

/*
 * Default constructor for Item
 *
 * Initializes an Item with placeholder values such as:
 * - name: "Item"
 * - description: "Description"
 * - rarity: 0
 * - all bonuses and multipliers: 0
 *
 * @param id Unique identifier for the item
 * @return none
 */
Item::Item() {

    // implementation of the item class default constructor

    // implementation of the item class overloaded constructor
    // in the order of (id, name, description, property)

    this->id = 0;

    this->name = "Item";
    this->description = "Description";
    this->rarity = 0;

    this->counter = 0;

    this->bonusStaminaMax = 0;
    this->bonusRationRegen = 0;
    this->bonusFov = 0;
    this->bonusRationCapacity = 0;
    this->bonusPickaxeCapacity = 0;

    this->bonusStaminaMaxMult = 0.0f;
    this->bonusRationRegenMult = 0.0f;
    this->bonusFovMult = 0.0f;
    this->bonusRationCapacityMult = 0.0f;
    this->bonusPickaxeCapacityMult = 0.0f;
}

/*
 * Overloaded constructor for Item
 *
 * Initializes an Item with specific values provided by parameters.
 * All bonus stats are initialized to zero. Typically used when reading from
 * file.
 *
 * @param id          Unique identifier for the item
 * @param name        Name of the item
 * @param description Short description of the item
 * @param property    Integer representing item rarity or other property
 * @return none
 */
Item::Item(int id, const std::string &name, const std::string &description,
           int rarity, bool hasCustomBehavior,
           const std::vector<int> &flatBonus, const std::vector<float> &mult) {

    this->id = id;
    this->name = name;
    this->description = description;
    this->rarity = rarity;
    this->hasCustomBehavior = hasCustomBehavior;

    this->counter = 0;

    this->bonusStaminaMax = flatBonus[0];
    this->bonusRationRegen = flatBonus[1];
    this->bonusFov = flatBonus[2];
    this->bonusRationCapacity = flatBonus[3];
    this->bonusPickaxeCapacity = flatBonus[4];

    this->bonusStaminaMaxMult = mult[0];
    this->bonusRationRegenMult = mult[1];
    this->bonusFovMult = mult[2];
    this->bonusRationCapacityMult = mult[3];
    this->bonusPickaxeCapacityMult = mult[4];
}
/*
 * Custom item behavior override
 *
 * Defines logic for items that have dynamic effects on the player.
 * Only executed if hasCustomBehavior is set to true.
 * Empty by default — override in derived classes if needed.
 *
 * @param player Reference to the Player object affected by the item
 * @return void
 */
void Item::update(Player &player) {
    switch (this->id) {
    case ItemID::WornSneakers:
        if (player.getPrevPos() != player.getPos() && rand() % 10 < 1)
            player.setStamina(player.getStamina() + 1);
        break;
    case ItemID::Wheelchair:
        if (player.getPrevPos() != player.getPos() && rand() % 4 < 1)
            player.setStamina(player.getStamina() + 1);
        break;
    case ItemID::HermesBoots:
        if (player.getPrevPos() != player.getPos() && rand() % 10 < 8)
            player.setStamina(player.getStamina() + 1);
        break;
    case ItemID::LitTorch:
        if (player.getPrevPos() != player.getPos())
            this->counter += 1;
        if (this->counter >= 150) {
            this->bonusFov = 0;
            this->name = "Smoldering Torch";
            this->description = "It has burnt out";
        }
        break;
    case ItemID::EmergencyRations:
        if (player.getStamina() == 0) {
            player.setStamina(player.getStaminaMax());
            this->name = "Emergency Rations (Consumed)";
            this->description = "It's eaten already";
        }
        break;
    case ItemID::MutatedPotato:
        if (player.getPrevPos() != player.getPos())
            this->counter++;
        if (this->counter >= 60) {
            if (player.hasItem(ItemID::SuspiciousPills))
                player.setStamina(player.getStamina() +
                                  int(player.getStaminaMax() * 0.1f));
            else if (player.getRationsOwned() < player.getRationCapacity())
                player.setRationsOwned(player.getRationsOwned() + 1);
            else
                break;
            this->counter = 0;
        }
        break;
    }
}
