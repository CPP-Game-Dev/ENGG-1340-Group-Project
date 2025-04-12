#include "../src/include/vector2d.h"
#include "../src/include/enums.h"
#include <cmath> // For distance calculation
#include <iostream>
#include <ncurses.h>
#include <vector>
#define MEGAPIXEL "  "
char getTileChar(TileObject tile) {
    switch (tile) {
    case TileObject::Player:
        return 'P';
    case TileObject::Wall:
        return '#';
    case TileObject::None:
        return '.';
    case TileObject::Exit:
        return 'E';
    case TileObject::Ration:
        return 'R';
    case TileObject::EnergyDrink:
        return 'D';
    case TileObject::Battery:
        return 'B';
    case TileObject::Pickaxe:
        return 'X';
    case TileObject::Chest:
        return 'C';
    case TileObject::Mimic:
        return 'M';
    default:
        return '?'; // Unknown tile
    }
}

int getTileColor(TileObject tile) {
    switch (tile) {
    case TileObject::Player:
        return 1; // Player as Yellow
    case TileObject::Wall:
        return 2; // Wall as White
    case TileObject::None:
        return 3; // None as Black
    case TileObject::Exit:
        return 4; // Exit as Red
    default:
        return 5; // Other tiles as Green
    }
}
int mapfov(int fov) {
    if (fov == 0 || fov == 1) {
        return 0;
    } else {
        if (fov % 2 == 0) {
            return fov - 1;
        } else {
            return fov - 2;
        }
    }
}

bool checkValid(int playerX, int playerY, int fov) {
    return playerX >= 0 && playerY >= 0 && fov >= 0;
}

void printMap(const std::vector<std::vector<TileObject> > &map, int playerX,
              int playerY, int fov) {
    for (int i = 0; i < (int)map.size(); i++) {
        for (int j = 0; j < (int)map[i].size(); j++) {
            if (i >= playerY - fov && i <= playerY + fov &&
                j >= playerX - fov && j <= playerX + fov) {
                // Tile is within the field of view
                int tile_color = getTileColor(map[i][j]);

                attron(COLOR_PAIR(tile_color));
                
                if (map[i][j] == TileObject::Wall || map[i][j] == TileObject::Player ||
                    map[i][j] == TileObject::None || map[i][j] == TileObject::Exit) {
                    addstr(MEGAPIXEL);; // Print two spaces as a "block"
                } else {
                    char t = getTileChar(map[i][j]);
                    printw("%c ", t);
                }

                attroff(COLOR_PAIR(tile_color));
            } else {
                // Tile Outside Field of view
                attron(COLOR_PAIR(6));
                addstr(MEGAPIXEL);
                attroff(COLOR_PAIR(6));
            }
        }
        printw("\n");
    }
}

void TesterProgram() {
    std::vector<std::vector<TileObject> > map = 
    {
        {TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::None, TileObject::None, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::Player, TileObject::None, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::None, TileObject::Exit, TileObject::Wall},
        {TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall}
    };

    int playerX = 2; // Player's X position in the map
    int playerY = 2; // Player's Y position in the map
    int fieldOfView = 5; // Field of view (player as center, near n blocks displayed)

    // Initialize ncurses
    initscr();
    start_color(); // Enable color functionality

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Player
    init_pair(2, COLOR_WHITE, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_BLACK, COLOR_RED);    // Exit
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // Extras
    init_pair(6, COLOR_BLACK, COLOR_CYAN);   // Hidden
    // Print the map with the player's field of view
    printMap(map, playerX, playerY, mapfov(fieldOfView));

    // Refresh the screen to show the map
    refresh();

    // Sample proof-of-concept to get input
    while (true) {
        char ch = getch();

        if (ch == 'w') {
            printw("Forward");
        } else if (ch == 'a') {
            printw("left");
        } else if (ch == 's') {
            printw("back");
        } else if (ch == 'd') {
            printw("right");
        } else if (ch == 'q') {
            break;
        } else {
            printw("Invalid input");
        }
    }
    //
    // Deallocate memory and end ncurses
    endwin();
}

int main() {
    TesterProgram();
    /*
        Gamemethod: printmap -> getch -> move(modification of tiles) ->get
       player pos -> printmap -> REPEAT
    */
    return 0;
}
