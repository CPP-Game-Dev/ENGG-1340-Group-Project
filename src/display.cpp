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
 *
 * @return void
 */
void Display::flush() {
    clear();
    refresh();
}

/*
 * Maps a TileObject to a string representation
 *
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
        return ">>";
    case TileObject::Ration:
        return "RA";
    case TileObject::EnergyDrink:
        return "EN";
    case TileObject::Chest:
        return "{}";
    case TileObject::Pickaxe:
        return "PX";
    default:
        return "??"; // Unknown tile
    }
}

/*
 * Maps a TileObject to its color pair ID
 *
 * @param tile TileObject enum
 * @return int representing ncurses color pair
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
 * Normalizes field of view value for consistent rendering
 *
 * @param fov raw field of view
 * @return int adjusted field of view for calculations
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
 * Draw a HUD with level data & background
 * @param player Player
 * @param currentLevel the number of levels completed - 1
 */
#pragma region DRAW LEVEL HUD
void drawLevelHUD(const Player &player, int currentLevel) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int height = 26, width = 23;
    Vector2D anchor = Vector2D(int(maxY / 2) + 1, int(maxX / 2));

    // Draw background
    attron(COLOR_PAIR(6) | A_DIM);
    for (int i = 0; i < 21; i++) {
        move(anchor.y - 10 + i,
             anchor.x - 19); // Move one character right to avoid left border
        for (int j = 0; j < 19; j++) { // Reduced from 21 to 19 fog blocks
            addstr(FOG);
        }
    }
    attroff(COLOR_PAIR(6) | A_DIM);

    // Rest of the function unchanged
    // Draw border
    attron(A_BOLD);
    attron(COLOR_PAIR(7));
    // Left
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 14 + i, anchor.x - 20, " |");

    // Right
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 14 + i, anchor.x + 20, "| ");

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

    attron(COLOR_PAIR(7));
    mvaddstr(anchor.y - 13, anchor.x - 20, level.c_str());
    mvaddstr(anchor.y - 13, anchor.x + 2, pickaxes.c_str());
    mvaddstr(anchor.y - 12, anchor.x - 20, stamina.c_str());
    mvaddstr(anchor.y - 12, anchor.x + 2, rations.c_str());
    attroff(COLOR_PAIR(7));
}

/*
 * Draw a HUD behind the current screen
 * @return void
 */
void drawHUD() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int height = 26, width = 23;
    Vector2D anchor = Vector2D(int(maxY / 2) + 1, int(maxX / 2));

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

    // Bottom
    move(anchor.y + 11, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // 4 Corners
    mvaddstr(anchor.y - 14, anchor.x - 22, " +");
    mvaddstr(anchor.y - 14, anchor.x + 22, "+ ");
    mvaddstr(anchor.y + 11, anchor.x - 22, " +");
    mvaddstr(anchor.y + 11, anchor.x + 22, "+ ");

    attroff(COLOR_PAIR(7));
    attroff(A_BOLD);
}
#pragma endregion

/*
 * Draws the current state of the maze, centered around the player
 * Applies FOV logic and colors based on tile type
 *
 * @param level Level object representing the current map
 * @param player Player object used for position and FOV
 * @return void
 */
#pragma region DRAW LEVEL
void Display::drawLevel(const Level &level, const Player &player,
                        int currentLevel) {
    clear();
    TileMap maze = level.getMaze();
    int size = level.getSize();
    int playerY = player.getPos().y, playerX = player.getPos().x;
    int fov = 4;

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2) + 1, int(maxX / 2));

    auto isPerimeter = [](int y, int x, int size) {
        return (y == -1 || x == -1 || y == size || x == size);
    };

    drawLevelHUD(player, currentLevel);

    for (int i = -1; i <= size; i++) {
        for (int j = -1; j <= size; j++) {
            // Fix: Position cursor correctly with proper tile offsets
            move(anchor.y + i - playerY, anchor.x + (j - playerX) * 2);
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

                // Calculate the screen coordinates before drawing
                int screenY = anchor.y + i - playerY;
                int screenX = anchor.x + (j - playerX) * 2;

                // Skip drawing fog if we're at a border location
                if (screenX == anchor.x - 20 || screenX == anchor.x + 20) {
                    continue;
                }

                move(screenY, screenX);
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

#pragma region DRAW MENU
void drawMenu(std::vector<std::string> options, int highlighted, int dy = 0,
              int dx = 0) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2), int(maxX / 2));
    anchor.y -= int(options.size() / 2);
    anchor.y += dy;

    // Keep track of visible options for highlighting
    int visibleIndex = 0;

    for (int i = 0; i < options.size(); i++) {
        if (options[i] == "\n") { // Special case, creates an empty row instead
            continue;
        }

        int len = strlen(options[i].c_str());
        // Use visibleIndex for highlighting calculation instead
        if (visibleIndex == highlighted % options.size()) {
            attron(COLOR_PAIR(7));
            mvprintw(anchor.y + i, int(anchor.x - len / 2) + dx, "< %s >",
                     options[i].c_str());
            attroff(COLOR_PAIR(7));
        } else {
            attron(COLOR_PAIR(5) | A_BOLD);
            mvprintw(anchor.y + i, int(anchor.x - len / 2) + dx, " <%s> ",
                     options[i].c_str());
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
        visibleIndex++;
    }
    refresh();
}
#pragma endregion

void Display::drawMainMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"New Game", "Help", "Settings", "Exit"};
    drawMenu(options, highlighted);
}

void Display::drawDifficultyMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Catacombs", "Labyrinth", "Purgatory",
                                        "Back"};

    drawMenu(options, highlighted);
}

void Display::drawPauseMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Continue", "New Game", "Inventory",
                                        "Help",     "Settings", "Exit"};
    drawMenu(options, highlighted);
}

void Display::drawGameOverMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Start Over", "Exit"};

    drawMenu(options, highlighted);
}

void Display::drawHelpMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Back"};

    drawMenu(options, highlighted);
}

void Display::drawInventoryMenu(
    int highlighted, const std::vector<std::unique_ptr<Item> > &inventory) {
    drawHUD();
    std::vector<std::string> options(5, "Empty");
    for (int i = 0; i < inventory.size(); i++)
        options[i] = inventory[i]->name;

    options.push_back("\n");
    options.push_back("Back");

    drawMenu(options, highlighted);
}

void Display::drawItemMenu(int highlighted, std::string desc) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2), int(maxX / 2));
    int len = strlen(desc.c_str());
    attron(COLOR_PAIR(5));
    mvprintw(anchor.y - 1, int(anchor.x - len / 2), "%s", desc.c_str());
    attroff(COLOR_PAIR(5));
    std::vector<std::string> options = {"Discard", "Back"};
    drawMenu(options, highlighted);
}

/*
 * Terminates ncurses session and restores terminal
 *
 * @return void
 */
void Display::terminate() { endwin(); }
