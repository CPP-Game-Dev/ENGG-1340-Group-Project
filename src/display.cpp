#include "include/display.h"
#include "include/level.h"
#include "include/vector2d.h"
#include "include/enums.h"
#include <assert.h>
#include <ncurses.h>


#define MEGAPIXEL "##"
#define BLANKSPACE "  "

/*
 * Function to initialize nCurses
 */
void Display::initCurses() {
    initscr();
    cbreak();
    start_color(); // Enable color functionality
    noecho();
    raw();
    //keypad(stdscr, true);

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Player
    init_pair(2, COLOR_WHITE, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_BLACK, COLOR_RED);    // Exit
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // Extras
    init_pair(6, COLOR_BLACK, COLOR_CYAN);   // Hidden
}

/*
 * Function to flush(clear) the screen
 */
void Display::flush() {
    clear();
    refresh();
}

void Display::drawLevel(Level level) {
    // TODO(Chris): Implement Manhatton distance based radius calculation
    //              & related drawing calculations

    // Initialize an anchor point (topleft of the maze)
    Vector2D maxSize;
    
    getmaxyx(stdscr, maxSize.y, maxSize.x);
    Vector2D anchor = Vector2D(int((maxSize.y - level.getSize()) / 2),
                               int((maxSize.x - level.getSize()) / 2));
    TileMap maze = level.getMaze();

    for (int i = 0; i < level.getSize(); i++) {
        move(anchor.y + i, anchor.x);
        for (int j = 0; j < level.getSize(); j++) {
            int colors = 0;
            bool is_items = false;
            switch (maze[i][j]) {
                case TileObject::Player:
                    colors = 1;
                    break;
                case TileObject::Wall:
                    colors = 2;
                    break;
                case TileObject::None:
                    colors = 3;
                    break;
                case TileObject::Exit:
                    colors = 4;
                    break;
                default:
                    colors = 5;
                    is_items = true;
                    break;
                }
            
            attron(COLOR_PAIR(colors));
            if(!is_items){
                addstr(BLANKSPACE);
            }else{
                switch(maze[i][j]){
                    case TileObject::Chest:
                        addstr("[]");
                        break;
                    case TileObject::Ration:
                        addstr("8");
                        break;
                    case TileObject::EnergyDrink:
                        addstr("E");
                        break;
                    case TileObject::Battery:
                        addstr("B");
                        break;
                }
            }
            attroff(COLOR_PAIR(colors));
        }
    }
}

void Display::terminate() {
    endwin();
}

