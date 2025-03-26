#include "include/display.h"
#include "include/level.h"
#include "vector2d.h"
#include <ncurses.h>

#define MEGAPIXEL "##"

Display::Display() {
    // TODO:Implementation
}

/*
* Function to initialize nCurses
*/
void Display::initCurses() {
    initscr();
    start_color(); // Enable color functionality
    noecho();
    raw();
    keypad(stdscr, true);

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Player
    init_pair(2, COLOR_WHITE, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_GREEN, COLOR_BLACK);  // Extras
    init_pair(5, COLOR_BLACK, COLOR_CYAN);   // Hidden

}

/*
* Function to flush(clear) the screen
*/
void Display::flush() {
    clear();
    refresh();
}

void Display::drawLevel(Level level) {
    // Initialize an anchor point (topleft of the maze)
    Vector2D maxSize;
    getmaxyx(stdscr, maxSize.y, maxSize.x);
    Vector2D anchor = Vector2D( int((maxSize.y - level.getSize())/2), 
                                int((maxSize.x - level.getSize())/2));
    std::vector<std::vector<TileObject>> maze = level.getMaze();

    auto getColors = [](TileObject tile){
        switch(tile){
            case TileObject::Player: return 1;
            case TileObject::Wall: return 2;
            case TileObject::None: return 3;
            default: return 4;
        }
    };

    for(int i = 0; i < level.getSize(); i++) {
        move(anchor.y + i, anchor.x);
        for(int j = 0; j < level.getSize(); j++) {
            int colors = getColors(maze[i][j]);
            attron(COLOR_PAIR(colors));
            addstr(MEGAPIXEL);
            attroff(COLOR_PAIR(colors));
        }
    }
    
}
