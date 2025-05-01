#pragma once

#include <vector>

/*
File to store all enums used in other classes
*/
enum GameState {
    MainMenu,
    DifficultyMenu,
    HelpMenu,
    SettingsMenu,
    InLevel,
    PauseMenu,
    InventoryMenu,
    ItemMenu,
    GameOverMenu
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
    Pickaxe,     // Doesn't do anything for now
    Chest,       // Gives random item
    Ink          // Marks walked tiles
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
    Right, 
    Confirm,
    Cancel,
    UseAbility,
    UsePickaxe,
    UseRation,
    Exit,
    None
};