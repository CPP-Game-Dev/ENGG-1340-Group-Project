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
  public:
    Main() {
        // TODO:Implementation
    }

    /*
     * Loads save data into the game
     * Throws error when there isn't one
     */
    void loadSave() {
        // TODO:Implementation
    }

    /*
     * Updates an existing save with new data
     * If there isn't an existing save, throw an error
     */
    void updateSave() {
        // TODO:Implementation
    }

    /*
     * Create a new save with default values
     */
    void newSave() {}

    /*
     * Handles all inputs and interpret them
     * Also handles screen resizing
     * Returns: the key pressed as an enum
     */

    // Main game loop
    void runGame() {
        // TODO:Implementation
    }
};

// DRIVER CODE //
int main() {
    // TODO:Implementation
}
