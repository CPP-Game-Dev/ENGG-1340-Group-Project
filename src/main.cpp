#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <assert.h>
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
    KeyInput getInput() {
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
        player.setStamina(player.getStaminaMax() * player.getStaminaMaxMult());
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

        switch (difficulty) {
        case (Difficulty::Catacombs):
            if (completedLevels % 2 == 0) {
                currentMapSize += 2;
            }

            break;
        case (Difficulty::Labyrinth):
            currentMapSize += 1;
            break;
        case (Difficulty::Purgatory):
            currentMapSize += 2;
            break;
        }

        currentLevel = Level(currentMapSize, player.getPos(), 4);
        resetPlayerStats();
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
        } else if (key == KeyInput::Down) {
            newPos = player.getPos() + UNIT_VECTOR_Y;
        } else if (key == KeyInput::Left) {
            newPos = player.getPos() - UNIT_VECTOR_X;
        } else if (key == KeyInput::Right) {
            newPos = player.getPos() + UNIT_VECTOR_X;
        }

        if (!currentLevel.isValidMove(newPos)) {
            return;
        }

        player.setPos(newPos);

        if (currentLevel.getTile(newPos) == TileObject::Exit) {
            onLevelComplete();
            return;
        }

        player.setStamina(player.getStamina() - 1);
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
        // TODO(James): Implementation
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

            if (player.getStamina() <= 0) {
                Display::terminate();
                break;
            }

            movePlayer(key);

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
