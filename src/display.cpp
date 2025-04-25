#include "include/display.h"
#include "include/enums.h"
#include "include/level.h"
#include "include/vector2d.h"
#include <assert.h>
#include <cmath>
#include <ncurses.h>
#include <string>

#define MEGAPIXEL "  "

/*
 * Function to initialize nCurses
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
    init_pair(6, COLOR_BLACK, COLOR_CYAN);   // Hidden
}

/*
 * Function to flush(clear) the screen
 */
void Display::flush() {
    clear();
    refresh();
}

#pragma region Helper Functions for drawLevel
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

bool isVisible(int y1, int x1, int y2, int x2, int fov) {
    double dy = std::abs(y1 - y2), dx = std::abs(x1 - x2);
    return std::round(std::sqrt(dy * dy + dx * dx)) < fov;
}
#pragma endregion

void Display::drawLevel(const Level &level, const Player &player) {
    // TODO(Chris): Implement Manhatton distance based radius calculation
    //              & related drawing calculations
    clear();
    TileMap maze = level.getMaze();
    int size = level.getSize();
    int playerY = player.getPos().y, playerX = player.getPos().x;
    // int fov = player.getFov();
    int fov = mapfov(4);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Initialize top left anchor to center
    Vector2D anchor = Vector2D(int(maxY/2), int(maxX/2));

    auto isPerimeter = [](int y, int x, int size) {
        return (y == -1 || x == -1 || y == size || x == size);
    };
    


    for (int i = -1; i <= size; i++) {
        // Move cursor to 1 mp left of anchor point
        move(anchor.y + i - playerY, anchor.x - 2 - playerX*2);
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
                    maze[i][j] == TileObject::Wall ||
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
                attron(COLOR_PAIR(6));
                addstr(MEGAPIXEL);
                attroff(COLOR_PAIR(6));
            }
        }
        printw("\n");
    }

    const char *str = "P1"; 
    mvprintw(anchor.y, anchor.x, "%s", str); 
}

void Display::drawMainMenu(int highlighted, Difficulty *difficulty) {
    std::vector<std::string> mainMenuItems = {
        "Start",
        "Help",
        "Exit",
    };
    std::vector<std::string> difficultyMenuItems = {"Easy", "Medium", "Hard",
                                                    "Back"};
    ArrowDisplay mainMenu(mainMenuItems, 0, 0);
    ArrowDisplay difficultyMenu(difficultyMenuItems, 0, 0);
    size_t mainChoice = mainMenu.run();
    if (mainChoice >= 0 && mainChoice < mainMenuItems.size()) {
        printf("You selected: %s\n", mainMenuItems[mainChoice].c_str());
        if (mainChoice == 0) {
            size_t difficultyChoice = difficultyMenu.run();
            if (difficultyChoice >= 0 &&
                difficultyChoice < difficultyMenuItems.size()) {
                printf("You selected: %s\n",
                       difficultyMenuItems[difficultyChoice].c_str());

                switch (difficultyChoice) {
                case 0:
                    *difficulty = Difficulty::Catacombs;
                    break;
                case 1:
                    *difficulty = Difficulty::Labyrinth;
                    break;
                case 2:
                    *difficulty = Difficulty::Purgatory;
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
                }
            }
        }
    } else {
        printf("No selection made or menu canceled.\n");
    }
}

void Display::terminate() { endwin(); }

void ArrowDisplay::display() {
    for (int i = 0; i < options.size(); ++i) {
        move(y + i, x);
        if (i == selected) {
            attron(A_REVERSE); // Highlight selected option
            printw("> %s", options[i].c_str());
            attroff(A_REVERSE);
        } else {
            printw("  %s", options[i].c_str());
        }
    }
    refresh();
}

int ArrowDisplay::run() {
    initscr();
    clear();
    noecho();
    cbreak();             // Line buffering disabled
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(0);          // Hide cursor

    display();

    while (true) {
        int ch = getch();
        switch (ch) {
        case KEY_UP:
            if (selected > 0) {
                selected--;
                display();
            }
            break;
        case KEY_DOWN:
            if (selected < options.size() - 1) {
                selected++;
                display();
            }
            break;
        case '\n': // Enter key
            endwin();
            return selected;
        case 27: // ESC key
            endwin();
            return -1;
        }
    }
}
