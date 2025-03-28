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
  public:
    Main() {
        // TODO(Joe): Implement default constructor
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
        default:
            return KeyInput::Nothing;
        }
    }
    
    /*
    * Function to update player stats at the start of a turn
    * Set player stats to their respective base stats
    * Do not touch stats without a respective base stat
    */
    void updatePlayerStats() {
        // TODO(Joe): Implementation
    }

    /*
    * Function to update player inventory items
    * Calls the update() function of every item in the player's inventory
    */
    void updatePlayerInventory() {
        // TODO(Joe, after MVP): Implementation
    }

    /*
    * Function to hold all display related code
    * Note that game logic does not affect display
    */
    void draw() {
        // TODO(Chris): Implementation
    }

    // Function to move the player and handle stamina reduction
    void movePlayer() {
        // TODO(Joe): Implementation
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
    }
};

// DRIVER CODE //
int main() {
    // TODO(James):Implementation
}
