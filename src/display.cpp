#include "include/display.h"
#include "include/enums.h"
#include "include/item.h"
#include "include/level.h"
#include "include/vector2d.h"
#include <assert.h>
#include <cmath>
#include <ncurses.h>
#include <sstream>
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

#pragma region DRAW LEVEL HUD
/*
 * Draw a HUD with level data & background
 * @param player Player
 * @param currentLevel the number of levels completed - 1
 */
void drawLevelHUD(const Player &player, int currentLevel, std::string &text) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int height = 27, width = 23;
    Vector2D anchor = Vector2D(int(maxY / 2) + 2, int(maxX / 2));

    // Draw background
    attron(COLOR_PAIR(6) | A_DIM);
    for (int i = 0; i < 21; i++) {
        move(anchor.y - 10 + i,
             anchor.x - 20); // Move one character right to avoid left border
        for (int j = 0; j < 21; j++) { // Reduced from 21 to 19 fog blocks
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
        mvaddstr(anchor.y - 15 + i, anchor.x - 22, " |");

    // Right
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 15 + i, anchor.x + 22, "| ");

    // Top
    move(anchor.y - 15, anchor.x - 22);
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
    mvaddstr(anchor.y - 15, anchor.x - 22, " +");
    mvaddstr(anchor.y - 15, anchor.x + 22, "+ ");
    mvaddstr(anchor.y - 11, anchor.x - 22, " +");
    mvaddstr(anchor.y - 11, anchor.x + 22, "+ ");
    mvaddstr(anchor.y + 11, anchor.x - 22, " +");
    mvaddstr(anchor.y + 11, anchor.x + 22, "+ ");

    attroff(COLOR_PAIR(7));
    attroff(A_BOLD);

    std::string stamina = "SP: ", rations = "Rations: ",
                pickaxes = "Pickaxes: ", level = "Layer ", line;
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
    if (!text.compare("Energy Drink"))
        line.append("Consumed Energy Drink");
    else {
        line.append("Collected ");
        line.append(text);
    }

    attron(COLOR_PAIR(7));
    mvaddstr(anchor.y - 14, anchor.x - 10 - int(level.size() / 2),
             level.c_str());
    mvaddstr(anchor.y - 14, anchor.x + 10 - int(pickaxes.size() / 2),
             pickaxes.c_str());
    mvaddstr(anchor.y - 13, anchor.x - 10 - int(stamina.size() / 2),
             stamina.c_str());
    mvaddstr(anchor.y - 13, anchor.x + 10 - int(rations.size() / 2),
             rations.c_str());
    attroff(COLOR_PAIR(7));

    attron(COLOR_PAIR(1));
    if (!text.empty()) {
        mvaddstr(anchor.y - 12, anchor.x + 1 - int(line.size() / 2),
                 line.c_str());
        text = "";
    }
    attroff(COLOR_PAIR(1));
}

/*
 * Draw a HUD behind the current screen
 * @return void
 */
void drawHUD() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int height = 27, width = 23;
    Vector2D anchor = Vector2D(int(maxY / 2) + 2, int(maxX / 2));

    // Draw border
    attron(A_BOLD);
    attron(COLOR_PAIR(7));
    // Left
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 15 + i, anchor.x - 22, " |");

    // Right
    for (int i = 0; i < height; i++)
        mvaddstr(anchor.y - 15 + i, anchor.x + 22, "| ");

    // Top
    move(anchor.y - 15, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // Bottom
    move(anchor.y + 11, anchor.x - 22);
    for (int i = 0; i < width; i++)
        addstr("--");

    // 4 Corners
    mvaddstr(anchor.y - 15, anchor.x - 22, " +");
    mvaddstr(anchor.y - 15, anchor.x + 22, "+ ");
    mvaddstr(anchor.y + 11, anchor.x - 22, " +");
    mvaddstr(anchor.y + 11, anchor.x + 22, "+ ");

    attroff(COLOR_PAIR(7));
    attroff(A_BOLD);
}
#pragma endregion

#pragma region DRAW LEVEL
/*
 * Renders the visible portion of the maze centered on the player
 * @param level current Level object
 * @param player current Player object
 * @param currentLevel index of current level (0 indexed)
 */
void Display::drawLevel(const Level &level, const Player &player,
                        int currentLevel, std::string &text,
                        KeyInput lastDirectionalInput) {
    clear();
    TileMap maze = level.getMaze();
    int size = level.getSize();
    int playerY = player.getPos().y, playerX = player.getPos().x;
    int fov = player.getFov();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2) + 2, int(maxX / 2));

    auto isPerimeter = [](int y, int x, int size) {
        return (y == -1 || x == -1 || y == size || x == size);
    };

    drawLevelHUD(player, currentLevel, text);

    for (int i = -1; i <= size; i++) {
        for (int j = -1; j <= size; j++) {
            move(anchor.y + i - playerY, anchor.x + (j - playerX) * 2);
            if (isVisible(i, j, playerY, playerX, fov) ||
                player.hasItem(ItemID::SunSeed)) {
                // Skip drawing if it's outside of HUD range
                if (i <= playerY - 10 || j <= playerX - 10 ||
                    j >= playerX + 10 || i >= playerY + 10) {
                    continue;
                }

                // Calculate the screen coordinates before drawing
                int screenY = anchor.y + i - playerY;
                int screenX = anchor.x + (j - playerX) * 2;

                // Skip drawing if we're at a border location
                if (screenX == anchor.x - 20 || screenX == anchor.x + 20) {
                    continue;
                }

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
    // attron(COLOR_PAIR(1));
    // if(!player.hasItem(ItemID::TelescopeGlasses))
    //     mvaddstr(anchor.y, anchor.x, "P1");
    // attroff(COLOR_PAIR(1));

    // Replace with:
    attron(COLOR_PAIR(1));
    if (!player.hasItem(ItemID::TelescopeGlasses)) {
        // Get character based on last direction
        std::string playerChar;
        switch (lastDirectionalInput) {
        case KeyInput::Right:
            playerChar = ">>";
            break;
        case KeyInput::Left:
            playerChar = "<<";
            break;
        case KeyInput::Up:
            playerChar = "^^";
            break;
        case KeyInput::Down:
            playerChar = "vv";
            break;
        default:
            playerChar = "P1"; // Default if no direction set yet
            break;
        }
        mvaddstr(anchor.y, anchor.x, playerChar.c_str());
    }
    attroff(COLOR_PAIR(1));
}
#pragma endregion

#pragma region DRAW MENU

/*
 * Displays a menu with selectable options and highlights current selection
 * @param options list of menu options
 * @param highlighted index of highlighted option
 * @return void
 */
void drawMenu(std::vector<std::string> options, int highlighted, int dy = 0,
              int dx = 0) {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2), int(maxX / 2));
    anchor.y -= int(options.size() / 2);
    anchor.y += dy;
    anchor.x -= 1;

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

/*
 * Displays the main menu with default options
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawMainMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"New Game", "Help", "Settings", "Exit"};
    drawMenu(options, highlighted);
}

/*
 * Displays the difficulty selection menu
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawDifficultyMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Catacombs", "Labyrinth", "Purgatory",
                                        "Back"};

    drawMenu(options, highlighted);
}

/*
 * Displays the pause menu during gameplay
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawPauseMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Continue", "New Game", "Inventory",
                                        "Help",     "Settings", "Exit"};
    drawMenu(options, highlighted);
}

/*
 * Displays the game over menu
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawGameOverMenu(int highlighted) {
    drawHUD();
    std::vector<std::string> options = {"Start Over", "Exit"};

    drawMenu(options, highlighted);
}

/*
 * Displays the help menu screen
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawHelpMenu(int highlighted) {
    drawHUD();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2), int(maxX / 2));

    std::vector<std::string> text = {"You're stuck in a dungeon without end.",
                                     "Moving around drains your stamina.",
                                     "Eat rations to regain stamina.",
                                     "Energy drinks has a similar effect,",
                                     "but cannot be carried.",
                                     "Equip yourself with items from chests.",
                                     "Use pickaxes to break through walls.",
                                     "And manage your rations carefully.",
                                     "Try your best to survive.",
                                     "We'll see how long you can last."};

    anchor.y -= int(text.size() / 2);

    attron(COLOR_PAIR(7) | A_ITALIC);
    for (int i = 0; i < text.size(); i++) {
        int len = strlen(text[i].c_str());
        // Use visibleIndex for highlighting calculation instead

        mvprintw(anchor.y + i, int(anchor.x - len / 2), " %s", text[i].c_str());
    }
    attroff(COLOR_PAIR(7) | A_ITALIC);

    std::vector<std::string> options = {"Back"};

    drawMenu(options, highlighted, int(text.size() / 2) + 1);
}

/*
 * Displays invetory screen
 * @param highlighted index of selected item
 * @return void
 */
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

/*
 * Displays item description menu
 * @param highlighted index of selected item
 * @return void
 */
void Display::drawItemMenu(int highlighted, std::string desc) {
    drawHUD();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY / 2) - 1, int(maxX / 2));
    std::stringstream ss(desc);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ss, line, '_'))
        lines.push_back(line);

    int dy = lines.size(), len;
    attron(COLOR_PAIR(5));
    for (int i = 0; i < dy; i++) {
        len = strlen(lines[i].c_str());
        mvprintw(anchor.y - dy + i, int(anchor.x - len / 2), "%s",
                 lines[i].c_str());
    }

    attroff(COLOR_PAIR(5));
    std::vector<std::string> options = {"Discard", "Back"};
    drawMenu(options, highlighted, 2);
}

#pragma region DRAW ANIMATION

/*
 * Displays the intro screen
 * @return void
 */
void Display::drawIntro() {
    wclear(stdscr);
    wrefresh(stdscr);
    drawScrollingText("Presented by Group 137...                   ",
                      std::make_pair(0, 0));
    usleep(500000);

    std::vector<std::string> title = {
        "       HH   HH  EEEEEEE  LL       LL       \n",
        "       HH   HH  EE       LL       LL       \n",
        "       HHHHHHH  EEEEE    LL       LL       \n",
        "       HH   HH  EE       LL       LL       \n",
        "       HH   HH  EEEEEEE  LLLLLLL  LLLLLLL  \n",
    };

    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    int start_y = (y_max - 10) / 2;
    int x_len = x_max / 2;
    int x_mod = x_max % 2;

    WINDOW *title_pad = newpad(9, 52);
    WINDOW *line_pad = newpad(2, x_len * 2 + x_mod);
    waddstr(line_pad, std::string(x_len * 2 + x_mod, '=').c_str());
    wattr_on(line_pad, COLOR_PAIR(6), nullptr);

    for (const auto &row : title) {
        waddstr(title_pad, row.c_str());
        wattr_on(title_pad, COLOR_PAIR(6), nullptr);
    }

    // Draw bars
    for (int i = 0; i < x_len + x_mod; i++) {
        prefresh(line_pad, 0, x_len - (i + 1) + x_mod, start_y,
                 x_len - (i + 1) + x_mod, start_y, x_len + i);
        prefresh(line_pad, 0, x_len - (i + 1) + x_mod, start_y + 9,
                 x_len - (i + 1) + x_mod, start_y + 9, x_len + i);
        usleep(20000);
    }

    WINDOW *logo_subwin = subwin(stdscr, 8, x_max, start_y + 1, 0);

    // Draw scanning effect
    int x_shift = (x_max - 52) / 2;
    for (int i = 0; i < 52 / 2 - 2; i++) {
        wclear(logo_subwin);
        wrefresh(logo_subwin);
        prefresh(title_pad, 0, i, start_y + 2, x_shift + i, start_y + 8,
                 x_shift + i);
        prefresh(title_pad, 0, 51 - i, start_y + 2, x_shift + 51 - i,
                 start_y + 8, x_shift + 51 - i);
        usleep(50000);
    }

    // Draw logo expansion
    for (int i = 3; i < 52 / 2; i++) {
        prefresh(title_pad, 0, 52 / 2 - (i + 1), start_y + 2,
                 x_shift + 52 / 2 - (i + 1), start_y + 8, x_shift + 52 / 2 + i);
        usleep(50000);
    }
    usleep(500000);
    flushinp();
    wclear(stdscr);
    wrefresh(stdscr);
}

/*
 * Displays scrolling text on the screen
 * @param text string to be displayed
 * @param pos position of the text
 * @param updates number of updates per second
 * @param mirrored if true, text will scroll in reverse
 * @param flush_screen if true, clears the screen before displaying
 * @param bold if true, text will be bold
 * @return void
 */
void Display::drawScrollingText(const std::string &text,
                                std::pair<int, int> pos, int updates,
                                bool mirrored, bool bold) {
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    if (pos.first == -1 || pos.second == -1) {
        pos = {y_max / 2, x_max / 2};
    }

    WINDOW *pad = newpad(10, 100);
    int start_x = pos.second - text.length() / 2;
    waddstr(pad, text.c_str());
    if (bold) {
        wattron(pad, COLOR_PAIR(6) | A_BOLD);
    } else {
        wattron(pad, COLOR_PAIR(6));
    }

    for (size_t i = 0; i < text.length(); i++) {
        if (!mirrored) {
            prefresh(pad, 0, 0, pos.first, start_x, pos.first, start_x + i);
        } else {
            prefresh(pad, 0, 0, pos.first, start_x + text.length() - i,
                     pos.first, start_x + text.length());
        }
        usleep(1000000 / updates);
    }

    mvwaddstr(stdscr, pos.first, pos.second, text.c_str());
    if (bold) {
        wattron(stdscr, COLOR_PAIR(6) | A_BOLD);
    } else {
        wattron(stdscr, COLOR_PAIR(6));
    }
    flushinp();
    usleep(500000);
    wclear(stdscr);
    wrefresh(stdscr);
}
#pragma endregion

#pragma region TERMINATE

/*
 * Terminates ncurses session and restores terminal
 * @return void
 */
void Display::terminate() { endwin(); }

#pragma endregion