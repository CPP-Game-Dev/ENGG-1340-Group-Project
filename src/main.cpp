#include "include/config.h"
#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/player.h"
#include "include/vector2d.h"

#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
#include <string>

#include "include/item.h"
#include "include/utils.h"

#define UNIT_VECTOR_Y Vector2D(1, 0)
#define UNIT_VECTOR_X Vector2D(0, 1)
#define SCREEN_SIZE_ERROR -1

/*
 * Class for everything related to the main game framework
 * Also handles all inputs
 * In short, this class puts everything together and handles the user's bs
 */

class Main {
  private:
    GameState gamestate;
    Player player;
    int currentMapSize = 5;
    float itemCount = 4.0f;
    int completedLevels = 0;
    int highlighted = 0;
    bool confirmed = false;
    KeyInput key = KeyInput::None;
    bool running = true;

    std::string selectedItemDesc;
    int selectedItemID;

    Difficulty difficulty;
    Level currentLevel;
    KeyInput lastDirectionalInput;
    Config config;


    std::vector<std::vector<std::unique_ptr<Item> > > unobtainedItems;

    /*
     * Opens the configuration file using the user's default editor
     * First checks the EDITOR environment variable, then falls back to vi
     * Temporarily exits ncurses mode to allow full terminal control
     *
     * @return void
     */
    void openConfigWithEditor() {
        // Get the config file path
        std::string configFile = config.getConfigFilePath();

        // Get the user's preferred editor from environment
        char *editorEnv = getenv("EDITOR");
        std::string editor;

        if (editorEnv && strlen(editorEnv) > 0) {
            editor = editorEnv;
        } else {
// Default editors by platform (prefer simpler editors)
#ifdef __APPLE__
            editor = "nano";
#else
            editor = "vi";
#endif
        }

        // Prepare to exit ncurses temporarily
        Display::terminate();

        // Construct and execute the command
        std::string command = editor + " " + configFile;
        std::cout << "Opening config with: " << command << std::endl;
        system(command.c_str());

        // Restore ncurses
        Display::initCurses();

        // Reload config
        config = Config();
    }

  public:
    /*
     * Default constructor for Main class
     * Sets up initial game state, level, player, and loads config & items
     *
     * @return none
     */
    Main() : currentLevel(currentMapSize, Vector2D(0, 0), 4) {
        gamestate = GameState::MainMenu;
        player = Player();
        player.setPos(0, 0);

        config = Config();

        utils::loadItems(unobtainedItems);

        // print contents of unobtainedItems
        //
    }

    /*
     * Handles all inputs and interpret them
     * Also handles screen resizing
     *
     * @return KeyInput
     */
#pragma region GET INPUT
    KeyInput getInput() { // TODO: Make it modular and configurable
        char inp = getch();

        if (inp == KEY_RESIZE) {
            assert(checkScreenSize());
            return KeyInput::None;
        } else if (inp == config.getConfig(Config::KB_UP)[0]) {
            return KeyInput::Up;
        } else if (inp == config.getConfig(Config::KB_DOWN)[0]) {
            return KeyInput::Down;
        } else if (inp == config.getConfig(Config::KB_LEFT)[0]) {
            return KeyInput::Left;
        } else if (inp == config.getConfig(Config::KB_RIGHT)[0]) {
            return KeyInput::Right;
        } else if (inp == config.getConfig(Config::KB_CONFIRM)[0]) {
            return KeyInput::Confirm;
        } else if (inp == config.getConfig(Config::KB_CANCEL)[0]) {
            return KeyInput::Cancel;
        } else if (inp == config.getConfig(Config::KB_USE_PICKAXE)[0]) {
            return KeyInput::UsePickaxe;
        } else if (inp == config.getConfig(Config::KB_USE_RATION)[0]) {
            return KeyInput::UseRation;
        } else if (inp == config.getConfig(Config::KB_EXIT)[0]) {
            return KeyInput::Exit;
        }

        return KeyInput::None;
    }
#pragma enderegion

    /*
     * Function to run when the user completes the current level
     * Adjusts level size and stamina based on difficulty
     * @return void
     */
#pragma region ON LEVEL COMPLETE
    void onLevelComplete() {
        player.setPos(Vector2D(0, 0));
        completedLevels++;

        double energyMult = 0;
        switch (difficulty) {
        case (Difficulty::Catacombs): // Easy mode
            if (completedLevels % 2 == 0) {
                currentMapSize += 2;
            }
            itemCount += 1.5f;
            energyMult = 0.75;
            break;
        case (Difficulty::Labyrinth): // Medium mode
            currentMapSize += 2;
            itemCount += 1.0f;
            energyMult = 0.55;
            break;
        case (Difficulty::Purgatory): // Hard mode
            currentMapSize += 3;
            itemCount += 0.75f;
            energyMult = 0.4;
            break;
        }
        if(player.hasItem(ItemID::GUEV1)) 
            itemCount += 2;

        currentLevel = Level(currentMapSize, player.getPos(), int(std::floor(itemCount)));
        auto newStamina =
            std::min(int(player.getStamina() +
                         std::floor(player.getStamina() * energyMult)),
                     int(player.getStamina() +
                         std::floor(player.getStaminaMax() * energyMult)));

        if(player.hasItem(ItemID::SleepingBag))
            newStamina += 10;
        if(player.hasItem(ItemID::EnchantedBracelet))
            newStamina += 15;
        if(player.hasItem(ItemID::AStar))
            player.setBaseStaminaMax(player.getBaseStaminaMax() + 10);

        player.setStamina(newStamina);
    }
#pragma endregion

    /*
     * Resets game & create new level
     * @return void
     */
    void newGame() {
        player.resetStats();
        completedLevels = 0;
        currentMapSize = 5;
        itemCount = 4.0f;
        if(difficulty == Difficulty::Purgatory)
            itemCount -= 1;
        currentLevel = Level(currentMapSize, player.getPos(), int(itemCount));
    }


    /*
     * Handles item pickup at a specific pos
     * Adds item to inventory and clears tile
     * @param pos Tile position where item was picked up
     * @return void
     */
#pragma region HANDLE COLLECTABLE INTERACTION
    void handleCollectableInteraction(Vector2D pos) {
        TileObject collectable = currentLevel.getTile(pos);
        bool pickup = false;
        switch (collectable) {
        case TileObject::Ration:
            if (player.getRationsOwned() < player.getRationCapacity()) {
                player.setRationsOwned(player.getRationsOwned() + 1);
                currentLevel.setTile(pos, TileObject::None);
                pickup = true;
            }
            break;
        case TileObject::Pickaxe:
            if (player.getPickaxesOwned() < player.getPickaxeCapacity()) {
                player.setPickaxesOwned(player.getPickaxesOwned() + 1);
                currentLevel.setTile(pos, TileObject::None);
                pickup = true;
            }
            break;
        case TileObject::EnergyDrink: {
            float mult = player.hasItem(ItemID::RedBull) ? 0.25f : 0.1f;
            if (int(player.getStamina() + player.getStaminaMax() * mult) <
                player.getStaminaMax()) {
                player.setStamina(
                    int(player.getStamina() + player.getStaminaMax() * mult));
                currentLevel.setTile(pos, TileObject::None);
                pickup = true;
            }
            break;
        }
        case TileObject::Chest:
            if (player.getItemCount() >= 5)
                break;

            // Check if we have any items at all
            bool hasItems = false;
            for (const auto &list : unobtainedItems) {
                if (!list.empty()) {
                    hasItems = true;
                    break;
                }
            }

            if (!hasItems) {
                std::cerr << "No items available!" << std::endl;
                break;
            }
            // Select a non-empty item list
            int rarity, weights[] = {50, 30, 15, 5}, rnd;
            do {
                rnd = rand() % 100;
                for (int i = 0; i < 4; i++) {
                    rnd -= weights[i];
                    if (rnd <= 0) {
                        rarity = 3;
                        break;
                    }
                }
            } while (unobtainedItems[rarity].empty());

            auto &itemList = unobtainedItems[rarity];
            int itemIndex = rand() % itemList.size();
            player.addItem(itemList[itemIndex], itemList);
            currentLevel.setTile(pos, TileObject::None);
            pickup = true;
            break;
        }

        if(pickup & player.hasItem(ItemID::CharmOfSatiation)) 
            player.setStamina(player.getStamina() + 5);

    }
#pragma enderegion
    /*
     * Moves player in the specified direction
     * Handles stamina cost and tile interaction
     *
     * @param key Direction input from player
     * @return void
     */
#pragma region MOVE PLAYER
    void movePlayer(KeyInput key) {
        Vector2D newPos = player.getPos();

        if (key == KeyInput::Up) {
            newPos = player.getPos() - UNIT_VECTOR_Y;
            lastDirectionalInput = KeyInput::Up;
        } else if (key == KeyInput::Down) {
            newPos = player.getPos() + UNIT_VECTOR_Y;
            lastDirectionalInput = KeyInput::Down;
        } else if (key == KeyInput::Left) {
            newPos = player.getPos() - UNIT_VECTOR_X;
            lastDirectionalInput = KeyInput::Left;
        } else if (key == KeyInput::Right) {
            newPos = player.getPos() + UNIT_VECTOR_X;
            lastDirectionalInput = KeyInput::Right;
        }

        lastDirectionalInput = key;

        if (!currentLevel.isValidMove(newPos)) { // Checks if it hits a wall
            return;
        }

        if (player.hasItem(ItemID::InkBottle) && currentLevel.getTile(player.getPos()) == TileObject::None)
            currentLevel.setTile(player.getPos(), TileObject::Ink);

        player.setPos(newPos);

        if (currentLevel.getTile(newPos) == TileObject::Exit) {
            onLevelComplete();
            return;
        }

        if (currentLevel.getTile(newPos) != TileObject::None &&
            currentLevel.getTile(newPos) != TileObject::Ink)
            handleCollectableInteraction(newPos);

        player.setStamina(player.getStamina() - 1);
    }
#pragma endregion
    /*
     * Function to break a wall
     * Checks if the player has a pickaxe and if the pickaxe capacity > 0
     * If so, check if the player is facing a wall,
     * the wall is removed (TileType set to None) and the pickaxe capacity is
     * reduced by 1
     * @return void
     */
#pragma region BREAK WALL
    void breakWall() {
        // Check if the player has a pickaxe, and pickaxe capacity > 0
        // Right now since there are no inventory checks yet, we will assume the
        // player has it
        if (player.getPickaxesOwned() > 0) {
            // Check if the player is facing a wall
            Vector2D newPos = player.getPos();
            if (lastDirectionalInput == KeyInput::Up) {
                newPos = player.getPos() - UNIT_VECTOR_Y;
            } else if (lastDirectionalInput == KeyInput::Down) {
                newPos = player.getPos() + UNIT_VECTOR_Y;
            } else if (lastDirectionalInput == KeyInput::Left) {
                newPos = player.getPos() - UNIT_VECTOR_X;
            } else if (lastDirectionalInput == KeyInput::Right) {
                newPos = player.getPos() + UNIT_VECTOR_X;
            }

            auto tile = currentLevel.getTile(newPos);

            if (tile == TileObject::Wall) {
                currentLevel.setTile(newPos, TileObject::None);
                player.setPickaxesOwned(player.getPickaxesOwned() - 1);
            }
        }
    }
#pragma endregion

    /*
     * Function to use a ration
     * Checks if the ration capacity > 0
     * If so, it will heal the player's stamina by the ration regen amount
     * @return void
     */
#pragma region USE RATION
    void useRation() {
        if (player.getRationsOwned() <= 0)
            return;

        auto newStamina =
            std::min(player.getStamina() + player.getRationRegen(),
                     player.getStaminaMax());

        player.setStamina(newStamina);
        if(player.hasItem(ItemID::BlueCheese) && rand() % 10 < 2)
            return;

        player.setRationsOwned(player.getRationsOwned() - 1);


    }
#pragma endregion
    /*
     * Function to check if screen size is too smalll
     * Throws out_of_range error if not
     * @return bool
     */
    bool checkScreenSize() {
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        return (maxY < 26 || maxX < 48);
    }

    /*
     * Function to handle menu options selection
     * @param highlighted The currently highlighted element
     * @param key Key input
     * @param optionsCount the number of options avaliable
     * @return void
     */
    void menuSelection(KeyInput key, int optionsCount) {
        if (key == KeyInput::Confirm)
            confirmed = true;
        else if (key == KeyInput::Up)
            highlighted -= 1;
        else if (key == KeyInput::Down)
            highlighted += 1;
        highlighted = (optionsCount + highlighted) % optionsCount;
    }

    void handleMainMenu() {
        Display::drawMainMenu(highlighted);
        // Player is still picking an option
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 4);
            return;
        }
        // Player pressed quit
        if (key == KeyInput::Exit) {
            running = false;
            return;
        }
        // Played confirmed their choice
        switch (highlighted) {
        case 0:          // New Game
            gamestate = GameState::DifficultyMenu;
            break;
        case 1: // Help Menu
            gamestate = GameState::HelpMenu;
            break;
        case 2:
            openConfigWithEditor();
            // You might want to redraw the screen or update the UI
            // after Redraw the main menu
            Display::drawMainMenu(highlighted);
            break;
        case 3: // Exit
            running = false;
            break;
        }
        // Reset highlighted item
        highlighted = 0;
        confirmed = false;
    }

    void handleDifficultyMenu() {
        Display::drawDifficultyMenu(highlighted);
        // Player is still picking an option
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 4);
            return;
        }

        // Played confirmed their choice
        if (key != KeyInput::Exit)
            gamestate = GameState::InLevel;
        switch (highlighted) {
        case 0: // Catacombs
            difficulty = Difficulty::Catacombs;
            break;
        case 1: // Labyrinth
            difficulty = Difficulty::Labyrinth;
            break;
        case 2: // Purgatory
            difficulty = Difficulty::Purgatory;
            break;
        case 3: // Back to main menu
            gamestate = GameState::MainMenu;
            break;
        }
        newGame();
        highlighted = 0;
        confirmed = false;
    }
    
    void handleHelpMenu() {
        Display::drawHelpMenu(highlighted);
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 1);
            return;
        }
        gamestate = GameState::MainMenu;
        highlighted = 0;
        confirmed = false;
    }
    
    void handleLevelLogic() {
        Display::drawLevel(currentLevel, player, completedLevels);

        key = getInput();

        player.preUpdate();
        player.update();
        // Check for key press
        if (key == KeyInput::Exit) {
            gamestate = GameState::PauseMenu;
            highlighted = 0;
            confirmed = false;
            return;
        }
        if (key == KeyInput::Up || key == KeyInput::Down ||
            key == KeyInput::Left || key == KeyInput::Right) {
            movePlayer(key);
        } else if (key == KeyInput::UsePickaxe)
            breakWall();
        else if (key == KeyInput::UseRation)
            useRation();



        if (player.getStamina() <= 0)
            gamestate = GameState::GameOverMenu;

        player.postUpdate();
    }
    
    void handleInventoryMenu() {
        Display::drawInventoryMenu(highlighted, player.getInventory());
        if (!confirmed) {
            key = getInput();
            // Use the maximum of either 5 or actual inventory size,
            // plus 1 for Back button
            menuSelection(key,
                          std::max(5, (int)player.getInventory().size()) + 1);
            return;
        }

        // When confirming a selection
        if (highlighted == std::max(5, (int)player.getInventory().size())) {
            // Selected the Back button
            gamestate = GameState::PauseMenu;
        } else if (highlighted < player.getInventory().size()) {
            // Selected an actual item
            selectedItemDesc = player.getInventory()[highlighted]->description;
            selectedItemID = player.getInventory()[highlighted]->id;
            gamestate = GameState::ItemMenu;
        }
        // Otherwise selected an empty slot, do nothing special

        highlighted = 0;
        confirmed = false;
    }
    
    void handleItemMenu() {
        Display::drawItemMenu(highlighted, selectedItemDesc);
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 2);
            return;
        }
        switch (highlighted) {
        case 0: // Discard item
            player.removeItem(selectedItemID, unobtainedItems);
            gamestate = GameState::InventoryMenu;
            break;
        case 1: // Back
            gamestate = GameState::InventoryMenu;
            break;
        }
        highlighted = 0;
        confirmed = false;
    }
    
    void handlePauseMenu() {
        Display::drawPauseMenu(highlighted);
        // Player is still picking an option
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 6);
            return;
        }
        // Player pressed quit
        if (key == KeyInput::Exit) {
            gamestate = GameState::InLevel;
            return;
        }
        // Played confirmed their choice
        switch (highlighted) {
        case 0: // Continue
            gamestate = GameState::InLevel;
            break;
        case 1: // New Game
            gamestate = GameState::DifficultyMenu;
            break;
        case 2: // Inventory menu
            gamestate = GameState::InventoryMenu;
            break;
        case 3: // Help Menu
            gamestate = GameState::HelpMenu;
            break;
        case 4: // Settings Menu
            gamestate = GameState::SettingsMenu;
            break;
        case 5: // Exit
            gamestate = GameState::MainMenu;
            break;
        }
        // Reset highlight & confirm
        highlighted = 0;
        confirmed = false;
    }

    void handleGameOverMenu() {
        Display::drawGameOverMenu(highlighted);
        // Player is still picking an option
        if (!confirmed) {
            key = getInput();
            menuSelection(key, 2);
            return;
        }
        // Player pressed quit
        if (key == KeyInput::Exit) {
            gamestate = GameState::MainMenu;
            return;
        }
        // Played confirmed their choice
        switch (highlighted) {
        case 0:          // Start Over
            gamestate = GameState::DifficultyMenu;
            break;
        case 1: // Exit
            gamestate = GameState::MainMenu;
            break;
        }
        // Reset highlighted item
        highlighted = 0;
        confirmed = false;
    }
    /*
     * Game Logic:
     * Game is not updated at all if no valid player input is detected
     * Otherwise, perform an action according to current gamestate & key input
     *
     * @return void
     */
    void runGame() {
        Display::initCurses();
        while (running) {
            switch (gamestate) {
            case GameState::MainMenu:
                handleMainMenu();
                break;

            case GameState::DifficultyMenu: 
                handleDifficultyMenu();
                break;
            
            case GameState::HelpMenu: {
                handleHelpMenu();
                break;
            }
            case GameState::InLevel:
                handleLevelLogic();
                break;

            case GameState::InventoryMenu:
                handleInventoryMenu();
                break;

            case GameState::ItemMenu:
                handleItemMenu();
                break;

            case GameState::PauseMenu:
                handlePauseMenu();
                break;

            case GameState::SettingsMenu:
                openConfigWithEditor();
                // Return to pause menu after editing config
                gamestate = GameState::PauseMenu;
                break;

            case GameState::GameOverMenu:
                handleGameOverMenu();
                break;
            }
            Display::flush();
        }
        Display::terminate();
    }
};

// DRIVER CODE //
int main() {
    srand(time(NULL));

    Main game = Main();
    game.runGame();
    return 0;
}
