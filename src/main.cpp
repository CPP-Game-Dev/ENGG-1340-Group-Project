#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

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

  public:
    Main() : currentLevel(currentMapSize, Vector2D(0, 0), 4) {
        gamestate = GameState::InLevel;
        player = Player();
        player.setPos(0, 0);
    }

    /*
     * Loads save data into the game
     * Throws error when there isn't one
     */
    void loadSave() {
        // TODO(Jenna, after MVP): Implementation
    }

    /*
     * Updates an existing save with new data
     * If there isn't an existing save, throw an error
     */
    void updateSave() {
        // TODO(Jenna, after MVP): Implementation
    }

    /*
     * Create a new save with default values
     */
    void newSave() {
        // TODO(Jenna, after MVP): Implementation
    }

    /*
     * Handles all inputs and interpret them
     * Also handles screen resizing
     * Returns: the key pressed as a KeyInput
     */
    KeyInput getInput() { // TODO: Make it modular and configurable
        char inp = getch();

        switch (inp) {
        case 'w':
            return KeyInput::Up;
        case 'a':
            return KeyInput::Left;
        case 's':
            return KeyInput::Down;
        case 'd':
            return KeyInput::Right;
        case 'q':
            return KeyInput::Quit;
        case 'e':
            return KeyInput::UsePickaxe;
        case 'r':
            return KeyInput::UseRation;
        default:
            return KeyInput::None;
        }
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
        player.setRationCapacity(player.getBaseRationCapacity() *
                                 player.getRationCapacityMult());
        player.setPickaxeCapacity(player.getBasePickaxeCapacity() *
                                  player.getPickaxeCapacityMult());
    }

    /*
     * Function to run when the user completes the current level
     *
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
        // TODO(James, after MVP): Implementation
    }

    /*
     * Function to hold all display related code
     * Note that game logic does not affect display
     */
    void draw() {
        // TODO(Chris): Implementation
    }

    // Function to move the player and handle stamina reduction
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

        if (currentLevel.getTile(newPos) ==
            TileObject::Exit) { // User completes the level
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

        int adjustedStamina =
            std::round(player.getStamina() * player.getRationRegen());

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

            if (key == KeyInput::Quit) { // TODO: Probably figure out a better
                                         // way to quit other than this.
                Display::terminate();
                break;
            }

            if (player.getStamina() <= 0) { // TODO: Lost game screen
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
                // TODO: Use ration
                useRation();
            }

            this->updatePlayerStats();
        }
    }
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
