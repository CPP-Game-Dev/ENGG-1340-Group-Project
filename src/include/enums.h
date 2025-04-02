#pragma once

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

enum Rarity
{
    Common,
    Uncommon,
    Rare,
    Relic
};


// Enum for different tile objects (anything that appears on the map is a tile
// object)
enum class TileObject {
    Player,         // Player
    Wall,           // Maze Wall
    None,           // Path
    Ration,         // Collectable Ration
    EnergyDrink,    // Max Stamina Boost (temporary) (also regenerates stamina)
    Battery,        // FOV boost (temporary)
    Pickaxe,        // Doesn't do anything for now
    Chest,          // Gives random item    
    Mimic           // Doesn't do anything for now
};

// Enum for the 3 levels of difficulty (to be implemented after MVP)
enum Difficulty {
    Catacombs,  // Easy
    Labyrinth,  // Medium
    Purgatory   // Hardx
};

// Enum to store meaningful keyboard inputs
enum KeyInput {
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
    Nothing
};
