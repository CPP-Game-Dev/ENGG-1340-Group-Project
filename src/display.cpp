#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/vector2d.h"
#include <assert.h>
#include <cmath>
#include <ncurses.h>
#include <string.h>
#include <string>

#define MEGAPIXEL "  "
#define FOG "::"

/*
 * Initializes the ncurses library and sets up color pairs for display
 *
 * @return void
 */
void Display::initCurses() {
    initscr();
    start_color(); // Enable color functionality
    cbreak();
    noecho();
    raw();
    curs_set(0);

    // Define color pairs
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Player
    init_pair(2, COLOR_WHITE, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_RED, COLOR_RED);      // Exit
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // Extras
    if (can_change_color()) {
        init_color(COLOR_CYAN, 32, 32, 32);
        init_color(COLOR_MAGENTA, 250, 150, 0);
    }
    init_pair(6, COLOR_WHITE, COLOR_BLACK);   // Fog
    init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Text
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK); // HUD text
}

/*
 * Clears and refreshes the screen
 * @return void
 */
void Display::flush() {
    clear();
    refresh();
}

/*
 * Maps a TileObject to a string representation
 * @param tile TileObject enum
 * @return std::string representation for display
 */
std::string getTileChar(TileObject tile) {
    switch (tile) {
    case TileObject::Player:
        return "P1";
    case TileObject::Wall:
        return "##";
    case TileObject::None:
        return "__";
    case TileObject::Exit:
        return "\\/";
    case TileObject::Ration:
        return "O ";
    case TileObject::EnergyDrink:
        return "RB";
    case TileObject::Battery:
        return "EN";
    case TileObject::Chest:
        return "[]";
    default:
        return "??"; // Unknown tile
    }
}

/*
 * Maps a TileObject to its color pair ID
 *
 * @param tile TileObject enum
 * @return int representing ncurses color
 */
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

/*
 * Adjusts field of view value to ensure odd and minimum values
 * @param fov raw field of view
 * @return int normalized field of view
 */
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

/*
 * Determines if a tile is within the player's visible radius
 * @param y1,y2,x1,x2 tile and player positions
 * @param fov player's field of view
 * @return bool true if visible, false otherwise
 */
bool isVisible(int y1, int x1, int y2, int x2, int fov) {
    double dy = std::abs(y1 - y2), dx = std::abs(x1 - x2);
    return std::round(std::sqrt(dy * dy + dx * dx)) <= fov;
}

/*
 * Draw a HUD behind the current screen
 * @param player Player
 * @param maxY, maxX Maximum size of screen
 * @param currentLevel the number of levels completed - 1
 */
#pragma region DRAW LEVEL HUD
void drawLevelHUD(const Player &player, const int maxY, const int maxX,
                  int currentLevel) {
    int height = 26, width = 23;
    Vector2D anchor = Vector2D(int(maxY / 2) + 1, int(maxX / 2));

    // Draw background
    attron(COLOR_PAIR(6) | A_DIM);
    for (int i = 0; i < 21; i++) {
        move(anchor.y - 10 + i, anchor.x - 20);
        for (int j = 0; j < 21; j++) {
            addstr(FOG);
        }
    }
    attroff(COLOR_PAIR(6) | A_DIM);

    // Draw border
    attron(A_BOLD);
    attron(COLOR_PAIR(7));
    // Left
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 14 + i, anchor.x - 22, " |");

    // Right
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 14 + i, anchor.x + 22, "| ");

    // Top
    move(anchor.y - 14, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // Middle
    move(anchor.y - 11, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // Bottom
    move(anchor.y + 11, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // 6 Corners
    mvaddstr(anchor.y - 14, anchor.x - 22, " +");
    mvaddstr(anchor.y - 14, anchor.x + 22, "+ ");
    mvaddstr(anchor.y - 11, anchor.x - 22, " +");
    mvaddstr(anchor.y - 11, anchor.x + 22, "+ ");
    mvaddstr(anchor.y + 11, anchor.x - 22, " +");
    mvaddstr(anchor.y + 11, anchor.x + 22, "+ ");

    attroff(COLOR_PAIR(7));
    attroff(A_BOLD);

    std::string stamina = "Stamina: ", rations = "Rations: ",
                pickaxes = "Pickaxes: ", level = "Layer ";
    stamina.append(std::to_string(player.getStamina()));
    stamina.append("/");
    stamina.append(std::to_string(player.getStaminaMax()));

    rations.append(std::to_string(player.getRationsOwned()));
    rations.append("/");
    rations.append(std::to_string(player.getRationCapacity()));

    pickaxes.append(std::to_string(player.getPickaxesOwned()));
    pickaxes.append("/");
    pickaxes.append(std::to_string(player.getPickaxeCapacity()));

    level.append(std::to_string(currentLevel));

    attron(COLOR_PAIR(8));
    mvaddstr(anchor.y - 13, anchor.x - 20, level.c_str());
    mvaddstr(anchor.y - 13, anchor.x + 2, pickaxes.c_str());
    mvaddstr(anchor.y - 12, anchor.x - 20, stamina.c_str());
    mvaddstr(anchor.y - 12, anchor.x + 2, rations.c_str());
    attroff(COLOR_PAIR(8));
}
#pragma endregion


 /*
 * Renders the visible portion of the maze centered on the player
 * @param level current Level object
 * @param player current Player object
 * @param currentLevel index of current level (0 indexed)
 */
#pragma region DRAW LEVEL
void Display::drawLevel(const Level &level, const Player &player,
                        int currentLevel) {
    clear();
    TileMap maze = level.getMaze();
    int size = level.getSize();
    int playerY = player.getPos().y, playerX = player.getPos().x;
    // int fov = player.getFov();
    int fov = 4;

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2) + 1, int(maxX / 2));

    auto isPerimeter = [](int y, int x, int size) {
        return (y == -1 || x == -1 || y == size || x == size);
    };

    /*
    10: -1
    11: -1, 0
    12: -1, 0, 1
    */

    drawLevelHUD(player, maxY, maxX, currentLevel);

    for (int i = -1; i <= size; i++) {
        // Move cursor to 1 mp left of anchor point
        move(anchor.y + i - playerY, anchor.x - 2 - playerX * 2);
        for (int j = -1; j <= size; j++) {
            if (isVisible(i, j, playerY, playerX, fov)) {
                if (isPerimeter(i, j, size)) {
                    // Tile inside FOV but is part of perimeter
                    attron(COLOR_PAIR(2));
                    addstr(MEGAPIXEL);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                // Tile is within the field of view & bounds of maze
                int tile_color = getTileColor(maze[i][j]);

                attron(COLOR_PAIR(tile_color));
                if (maze[i][j] == TileObject::Wall ||
                    maze[i][j] == TileObject::None ||
                    maze[i][j] == TileObject::Exit) {
                    addstr(MEGAPIXEL); // Print two spaces as a "block"
                } else {
                    std::string t = getTileChar(maze[i][j]);
                    addstr(t.c_str());
                }

                attroff(COLOR_PAIR(tile_color));
            } else {
                // Tile Outside Field of view
                if (i < playerY - 10 || j < playerX - 10 || j > playerX + 10 ||
                    i > playerY + 10) {
                    continue;
                }
                attron(COLOR_PAIR(6) | A_DIM);
                addstr(FOG);
                attroff(COLOR_PAIR(6) | A_DIM);
            }
        }
        // printw("\n");
    }
    mvaddstr(anchor.y, anchor.x, "P1");
}
#pragma endregion

/*
 * Displays a menu with selectable options and highlights current selection
 * @param options list of menu options
 * @param highlighted index of highlighted option
 * @return void
 */
#pragma region DRAW MENU
void drawMenu(std::vector<std::string> options, int highlighted, int dy = 0,
              int dx = 0) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2), int(maxX / 2));
    anchor.y -= int(options.size() / 2);
    for (int i = 0; i < options.size(); i++) {
        int len = strlen(options[i].c_str());
        if (i == highlighted % options.size()) {
            attron(COLOR_PAIR(7));
            mvprintw(anchor.y + i + dy, int(anchor.x - len / 2) + dx, "< %s >",
                     options[i].c_str());
            attroff(COLOR_PAIR(7));
        } else {
            attron(A_BOLD);
            attron(COLOR_PAIR(5) | A_BOLD);
            mvprintw(anchor.y + i + dy, int(anchor.x - len / 2) + dx, " <%s> ",
                     options[i].c_str());
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
    }
    refresh();
}
#pragma endregion

/*
 * Displays the main menu with default options
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawMainMenu(int highlighted) {
    std::vector<std::string> options = {"New Game", "Help", "Settings", "Exit"};
    drawMenu(options, highlighted);
}

/*
 * Displays the difficulty selection menu
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawDifficultyMenu(int highlighted) {
    std::vector<std::string> options = {"Catacombs", "Labyrinth", "Purgatory",
                                        "Back"};

    drawMenu(options, highlighted);
}

/*
 * Displays the pause menu during gameplay
 *
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawPauseMenu(int highlighted) {
    std::vector<std::string> options = {"Continue", "New Game", "Help",
                                        "Settings", "Exit"};
    drawMenu(options, highlighted);
}

/*
 * Displays the game over menu
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawGameOverMenu(int highlighted) {
    std::vector<std::string> options = {"Start Over", "Exit"};

    drawMenu(options, highlighted);
}
/*
 * Displays the help menu screen
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawHelpMenu(int highlighted) {
    std::vector<std::string> options = {"Back"};

    drawMenu(options, highlighted);
}

/*
 * Terminates ncurses session and restores terminal
 * @return void
 */
void Display::terminate() { endwin(); }
