#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <assert.h>
#include <chrono>
#include <ctime>
#include <fstream>
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

  public:
    Main() {
        // TODO(James): Implement default constructor
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
        // TODO(James): Implementation
        player.setFov(player.getFov() * player.getFovMult());
        player.setStamina(player.getStaminaMax() * player.getStaminaMaxMult());
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

        if (key == KeyInput::Up) {
            player.setPos(player.getPos() - UNIT_VECTOR_Y);
        } else if (key == KeyInput::Down) {
            player.setPos(player.getPos() + UNIT_VECTOR_Y);
        } else if (key == KeyInput::Left) {
            player.setPos(player.getPos() - UNIT_VECTOR_X);
        } else if (key == KeyInput::Right) {
            player.setPos(player.getPos() + UNIT_VECTOR_X);
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
        gamestate = GameState::InLevel;
        player = Player();
        player.setPos(5, 5);
        Level currentLevel = Level(15, player.getPos(), 4);
        KeyInput key = KeyInput::None;

        while (true) {
            if (gamestate == GameState::InLevel) {
                Display::drawLevel(currentLevel, player);
            }

            key = getInput();

            if (key == KeyInput::Quit) { // TODO: Probably figure out a better
                                         // way to quit other than this.
                break;
            }

            if (player.getStamina() <= 0) {
                break;
            }

            movePlayer(key);

            this->updatePlayerStats();
        }
    }
};

// DRIVER CODE //
int main() {
    // TODO(James):Implementation
    Main game = Main();
    game.runGame();
    return 0;
}
