
#include <string>
#include "include/item.h"
#include "include/enums.h"
#include "include/utils.h"



Item::Item(ItemID id) {
    this->id = id;

    this->name = "Item";
    this->description = "Description";
    this->rarity = 0;
    
    this->counter = 0;

    this->bonusStaminaMax = 0;
    this->bonusRationRegen = 0;
    this->bonusFov = 0;
    this->bonusRationCapacity = 0;
    this->bonusPickaxeCapacity = 0;
    
    this->bonusStaminaMaxMult = 0.0;
    this->bonusRationRegenMult = 0.0;
    this->bonusFovMult = 0.0;
    this->bonusRationCapacityMult = 0.0;
    this->bonusPickaxeCapacityMult = 0.0;
    setDefaults();
}

std::unique_ptr<Item> createItem(ItemID itemID) { return utils::make_unique<Item>(itemID); }

void Item::setDefaults() {
    switch(this->id) {
        // Common
        case ItemID::CampingBoots:

        case ItemID::OldBelt:

        case ItemID::HoardersBackpack:

        case ItemID::CharredSteak:

        case ItemID::RottenEgg:
        
        case ItemID::OilLamp:

        case ItemID::ExpiredMilk:

        case ItemID::WornSneakers:

        case ItemID::SpinachLeaf:

        case ItemID::InkBottle:

        case ItemID::OversizedCookie:

        case ItemID::MutatedPotato:

        case ItemID::BlueCheese:

        case ItemID::SleepingBag:

        // Uncommon
        case ItemID::CampingBackpack:

        case ItemID::CampingFlashlight:

        case ItemID::CharmOfSatiation:
        
        case ItemID::EmergencyRations:

        case ItemID::RollerSkates:

        case ItemID::LitTorch:

        case ItemID::MRE:

        case ItemID::MillitaryBackpack:

        case ItemID::EnchantedBracelet:

        case ItemID::GUEV1:
        
        case ItemID::GUEV2:

        case ItemID::BundleOfSpinach:

        // Rare
        case ItemID::Headlights:

        case ItemID::TelescopeGlasses:

        case ItemID::MetalDetector:

        case ItemID::MagicMushroom:

        case ItemID::MiningHelmet:

        case ItemID::SuspiciousPills:

        case ItemID::InsulinInjection:

        case ItemID::Wheelchair:

        case ItemID::HikingStaff:

        case ItemID::PremiumSportswear:

        // Relic
        case ItemID::HermesBoots:

        case ItemID::QuadEspresso:

        case ItemID::SunSeed:

        case ItemID::AStar:

        default:
            break;
    }
}