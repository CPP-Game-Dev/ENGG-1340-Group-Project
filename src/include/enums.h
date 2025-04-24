#pragma once

#include <vector>

/*
File to store all enums used in other classes
*/
enum GameState {
    StartMenu,
    HelpMenu,
    SettingsMenu,
    InLevel,
    PauseMenu,
    InventoryMenu
};

enum class ItemID {
    // Common
    CampingBoots,
    OldBelt,
    HoardersBackpack,
    CharredSteak,
    RottenEgg,
    OilLamp,
    ExpiredMilk,
    WornSneakers,
    SpinachLeaf,
    InkBottle,
    OversizedCookie,
    MutatedPotato,
    BlueCheese,
    SleepingBag,
    // Uncommon
    CampingBackpack,
    CampingFlashlight,
    CharmOfSatiation,
    EmergencyRations,
    RollerSkates,
    LitTorch,
    MRE,
    MillitaryBackpack,
    EnchantedBracelet,
    GUEV1,
    GUEV2,
    BundleOfSpinach,
    // Rare
    Headlights,
    TelescopeGlasses,
    MetalDetector,
    MagicMushroom,
    MiningHelmet,
    SuspiciousPills,
    InsulinInjection,
    Wheelchair,
    HikingStaff,
    PremiumSportswear,
    // Relic
    HermesBoots,
    QuadEspresso,
    SunSeed,
    AStar
};

// Enum for different tile objects (anything that appears on the map is a tile
// object)
enum class TileObject {
    Player,      // Player
    Wall,        // Maze Wall
    None,        // Path
    Exit,        // Exit
    Ration,      // Collectable Ration
    EnergyDrink, // Max Stamina Boost (temporary) (also regenerates stamina)
    Battery,     // FOV boost (temporary)
    Pickaxe,     // Doesn't do anything for now
    Chest,       // Gives random item
    Mimic        // Doesn't do anything for now
};

// Declares TileMap to wrap 2d vector of TileObject into a more readable form
typedef std::vector<std::vector<TileObject> > TileMap;

// Enum for the 3 levels of difficulty (to be implemented after MVP)
enum Difficulty {
    Catacombs, // Easy
    Labyrinth, // Medium
    Purgatory  // Hardx
};

// Enum to store meaningful keyboard inputs
enum class KeyInput {
    Up,
    Down,
    Left,
    Right, // Only this part matters for the MVP
    Confirm,
    Cancel,
    Pause,
    UseAbility,
    UsePickaxe,
    UseRation,
    Quit,
    None
};

// Enum to store current game status
enum class GameStatus {
    PRE_GAME,
    IN_GAME,
    GAME_LOST,
    GAME_WON,
};
