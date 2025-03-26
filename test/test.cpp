#include <cmath> // For distance calculation
#include <iostream>
#include <ncurses.h>
#include <vector>
using namespace std;

enum TileObject {
    Player,
    Wall,
    None, // Player, Wall of maze, Path
    RegenSmall,
    RegenMedium,
    RegenBig, // 3 tiers of instant Stamina replenish
    StaminaSmall,
    StaminaMedium,
    StaminaBig, // 3 tiers of maximum Stamina boost
    VisionSmall,
    VisionMedium,
    VisionBig // 3 tiers of vision range boost
};

char getTileChar(TileObject tile) {
    switch (tile) {
    case Player:
        return 'P';
    case Wall:
        return '#';
    case None:
        return '.';
    case RegenSmall:
        return 'r';
    case RegenMedium:
    case RegenBig:
        return 'R';
    case StaminaSmall:
        return 's';
    case StaminaMedium:
    case StaminaBig:
        return 'S';
    case VisionSmall:
        return 'v';
    case VisionMedium:
    case VisionBig:
        return 'V';
    default:
        return '?';
    }
}

int getTileColor(TileObject tile) {
    switch (tile) {
    case Player:
        return 1; // Player as Yellow
    case Wall:
        return 2; // Wall as White
    case None:
        return 3; // None as Green
    default:
        return 4; // Other tiles as Green
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

void printMap(const vector<vector<TileObject>> &map, int playerX, int playerY,
              int fov) {
    for (int i = 0; i < (int)map.size(); i++) {
        for (int j = 0; j < (int)map[i].size(); j++) {
            if (i >= playerY - fov && i <= playerY + fov &&
                j >= playerX - fov && j <= playerX + fov) {
                // Tile is within the field of view

                int tile_color = getTileColor(map[i][j]);
                attron(COLOR_PAIR(tile_color));

                if (map[i][j] == Wall || map[i][j] == Player ||
                    map[i][j] == None) {
                    printw("  "); // Print two spaces as a "block"
                } else {
                    char t = getTileChar(map[i][j]);
                    if (map[i][j] == RegenBig || map[i][j] == StaminaBig ||
                        map[i][j] == VisionBig) {
                        attron(A_BOLD);
                        printw("%c ", t);
                        attroff(A_BOLD);
                    } else {
                        printw("%c ", t);
                    }
                }

                attroff(COLOR_PAIR(tile_color));
            } else {
                // Tile Outside Field of view
                attron(COLOR_PAIR(5));
                printw("  ");
                attroff(COLOR_PAIR(5));
            }
        }
        printw("\n");
    }
}

void TesterProgram() {
    vector<vector<TileObject>> map = {
        {Wall, Wall, Wall, Wall, Wall, Wall, Wall},
        {Wall, StaminaBig, VisionSmall, None, None, None, Wall},
        {Wall, RegenMedium, Player, None, None, None, Wall},
        {Wall, None, None, None, None, None, Wall},
        {Wall, Wall, Wall, Wall, Wall, Wall, Wall}};

    int playerX = 2; // Player's X position in the map
    int playerY = 2; // Player's Y position in the map
    int fieldOfView =
        2; // Field of view (player as center, near n blocks displayed)

    // Initialize ncurses
    initscr();
    start_color(); // Enable color functionality

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Player
    init_pair(2, COLOR_BLACK, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_GREEN, COLOR_BLACK);  // Extras
    init_pair(5, COLOR_BLACK, COLOR_CYAN);   // Hidden

    // Print the map with the player's field of view
    printMap(map, playerX, playerY, mapfov(fieldOfView));

    // Refresh the screen to show the map
    refresh();

    // Pause the screen output
    getch();

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
