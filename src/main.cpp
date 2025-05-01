#include "include/config.h"
#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/player.h"
#include "include/vector2d.h"

#include <iostream>
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ncurses.h>

#include "include/utils.h"
#include "include/item.h"


#define UNIT_VECTOR_Y Vector2D(1, 0)
#define UNIT_VECTOR_X Vector2D(0, 1)

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
    int completedLevels = 0;
    Difficulty difficulty;
    Level currentLevel;
    KeyInput lastDirectionalInput;
    Config config;

    std::vector<std::unique_ptr<Item> > items;
    
    /*
     * Initializes the item list from CSV data
     * Called in constructor
     *
     * @return void
     */
    void initialiseItems() {

        items = utils::parseItemsFromCSV("data/items.csv");
    }

  public:
     /*
     * Default constructor for Main class
     * Sets up initial game state, level, player, and loads config & items
     *
     * @return none
     */
    Main() : currentLevel(currentMapSize, Vector2D(0, 0), 4) {
        gamestate = GameState::InLevel;
        player = Player();
        player.setPos(0, 0);

        config = Config();

        initialiseItems();
    }

     /*
     * Loads save data from file into game state
     * Throws an error if file not found
     *
     * @return void
     */
    void loadSave() {

        std::ifstream in("save.dat");

        if (!in.is_open()) {
            throw std::runtime_error(
                "Save file not found. Make sure to creae one using newSave().");
        }

        int x, y;
        int stamina;

        in >> x >> y >> stamina;

        // applied loaded values to the player object
        player.setPos(x, y);
        player.setStamina(stamina);

        in.close();
    }

     /*
     * Updates existing save file with current game state
     * Throws an error if file cannot be written
     *
     * @return void
     */
    void updateSave() {

        std::ofstream out("save.dat");

        if (!out.is_open()) {
            throw std::runtime_error("Failed to open save file for writing.");
        }

        Vector2D pos = player.getPos();
        int stamina = player.getStamina();

        // format: pos.x pos.y stamina
        out << pos.x << " " << pos.y << " " << stamina << std::endl;

        out.close();
    }

     /*
     * Creates a new save file with default values
     * Used when starting a new game
     * @return void
     */
    void newSave() {

        player.setPos(0, 0);

        // set default stamina based on and multiplier
        int initialStamina =
            player.getBaseStaminaMax() * player.getStaminaMaxMult();

        player.setStamina(initialStamina);

        updateSave();
    }

    /*
     * Handles all inputs and interpret them
     * Also handles screen resizing
     *
     * @return KeyInput
     */
    KeyInput getInput() { // TODO: Make it modular and configurable
        char inp = getch();

        if (inp == config.getConfig(Config::KB_UP)[0]) {
            return KeyInput::Up;
        } else if (inp == config.getConfig(Config::KB_DOWN)[0]) {
            return KeyInput::Down;
        } else if (inp == config.getConfig(Config::KB_LEFT)[0]) {
            return KeyInput::Left;
        } else if (inp == config.getConfig(Config::KB_RIGHT)[0]) {
            return KeyInput::Right;
        } else if (inp == config.getConfig(Config::KB_USE_PICKAXE)[0]) {
            return KeyInput::UsePickaxe;
        } else if (inp == config.getConfig(Config::KB_USE_RATION)[0]) {
            return KeyInput::UseRation;
        } else if (inp == config.getConfig(Config::KB_QUIT)[0]) {
            return KeyInput::Quit;
        }

        return KeyInput::None;
    }

    /*
     * Function to update player stats at the start of a turn
     * Set player stats to their respective base stats
     * Do not touch stats without a respective base stat
     */
    void updatePlayerStats() {
        player.setFov(player.getFov() * player.getFovMult());
        player.setRationCapacity(player.getRationCapacity() *
                                 player.getRationCapacityMult());
        player.setPickaxeCapacity(player.getPickaxeCapacity() *
                                  player.getPickaxeCapacityMult());
    }

    /*
     * Function to reset player stats at the start of a new level
     *
     */
    void resetPlayerStats() {
        player.setStamina(player.getBaseStaminaMax() *
                          player.getStaminaMaxMult());
        player.setFov(player.getBaseFov() * player.getFovMult());
    }

    /*
     * Function to run when the user completes the current level
     * Adjusts level size and stamina based on difficulty
     * @return void
     */
    void onLevelComplete() {
        player.setPos(Vector2D(0, 0));
        completedLevels++;

        double energyMult = 0;
        switch (difficulty) {
        case (Difficulty::Catacombs): // Easy mode
            if (completedLevels % 2 == 0) {
                currentMapSize += 2;
            }

            energyMult = 0.7;
            break;
        case (Difficulty::Labyrinth): // Medium mode
            currentMapSize += 2;
            energyMult = 0.5;
            break;
        case (Difficulty::Purgatory): // Hard mode
            currentMapSize += 3;
            energyMult = 0.3;
            break;
        }

        currentLevel = Level(currentMapSize, player.getPos(), 4);
        resetPlayerStats();
        auto newStamina =
            std::min((int)(player.getStamina() +
                           std::floor(player.getStamina() * energyMult)),
                     player.getStaminaMax());

        player.setStamina(newStamina);
    }

    /*
     * Function to update player inventory items
     * Calls the update() function of every item in the player's inventory
     */
    void updatePlayerInventory() {

       player.preUpdate();

    // Loop through player's inventory
    const auto& inventory = player.getInventory();

    for (const auto& item : inventory) {
        if (!item) continue;

        // If item defines custom logic, call it
        if (item->hasCustomBehavior) {
            item->update(player);
        }

        // Apply flat bonuses
        player.setStaminaMax(player.getStaminaMax() + item->bonusStaminaMax);
        player.setRationRegen(player.getRationRegen() + item->bonusRationRegen);
        player.setFov(player.getFov() + item->bonusFov);
        player.setRationCapacity(player.getRationCapacity() + item->bonusRationCapacity);
        player.setPickaxeCapacity(player.getPickaxeCapacity() + item->bonusPickaxeCapacity);

        // Apply stat multipliers
        player.setStaminaMaxMult(player.getStaminaMaxMult() * item->bonusStaminaMaxMult);
        player.setRationRegenMult(player.getRationRegenMult() * item->bonusRationRegenMult);
        player.setFovMult(player.getFovMult() * item->bonusFovMult);
        player.setRationCapacityMult(player.getRationCapacityMult() * item->bonusRationCapacityMult);
        player.setPickaxeCapacityMult(player.getPickaxeCapacityMult() * item->bonusPickaxeCapacityMult);
    }

      
    player.update();
        
    }

    /*
     * Function to hold all display related code
     * Note that game logic does not affect display
     */
    void draw() {
        // TODO(Chris): Implementation
    }

    /*
     * Moves player in the specified direction
     * Handles stamina cost and tile interaction
     *
     * @param key Direction input from player
     * @return void
     */
  void movePlayer(KeyInput key) {
    auto newPos = player.getPos();

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

    player.setPos(newPos);


    handleItemPickupAt(newPos);


    
    if (currentLevel.getTile(newPos) == TileObject::Exit) {
        onLevelComplete();
        return;
    }

    player.setStamina(player.getStamina() - 1);
}


    /*
     * Function to break a wall
     * Checks if the player has a pickaxe and if the pickaxe capacity > 0
     * If so, check if the player is facing a wall,
     * the wall is removed (TileType set to None) and the pickaxe capacity is
     * reduced by 1
     *
     */
    void breakWall() {
        // Check if the player has a pickaxe, and pickaxe capacity > 0
        // Right now since there are no inventory checks yet, we will assume the
        // player has it
        // TODO: Add inventory checks later
        if (player.getPickaxeCapacity() > 0) {
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
                player.setPickaxeCapacity(player.getPickaxeCapacity() - 1);
            }
        }
    }

    /*
     * Function to use a ration
     * Checks if the ration capacity > 0
     * If so, it will heal the player's stamina by the ration regen amount
     *
     */
    void useRation() {
        if (player.getRationCapacity() <= 0) {
            return;
        }

        auto newStamina =
            std::min(player.getStamina() + player.getRationRegen(),
                     player.getStaminaMax());

        player.setStamina(newStamina);
    }

    /*
     * Game Logic:
     * Game is not updated at all if no valid player input is detected
     * Otherwise, perform an action according to current gamestate & key input
     * player stats are
     *
     */

    // Main game loop
    void runGame() {
        Display::initCurses();
        KeyInput key = KeyInput::None;

        while (true) {
            if (gamestate == GameState::InLevel) {
                Display::drawLevel(currentLevel, player);
            }

            key = getInput();

            if (key == KeyInput::Quit) {
                Display::terminate();
                break;
            }

            if (player.getStamina() <= 0) {
                Display::terminate();
                break;
            }

            // Check for key type
            if (key == KeyInput::Up || key == KeyInput::Down ||
                key == KeyInput::Left || key == KeyInput::Right) {
                movePlayer(key);
            } else if (key == KeyInput::UsePickaxe) {
                breakWall();
            } else if (key == KeyInput::UseRation) {
                useRation();
            }

            this->updatePlayerStats();
        }
    }
    /*
     * Draws the main menu and handles difficulty selection
     * @return void
     */
    void runMenu() { Display::drawMainMenu(0, &difficulty); }
};

// DRIVER CODE //
int main() {
    srand(time(NULL));

    Main game = Main();
    game.runMenu();
    game.runGame();
    return 0;
}
