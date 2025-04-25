#pragma once

#include "enums.h"
#include "level.h"
#include "player.h"

/*
 * Class to store functions to draw the game on the console
 */
class Display {
  public:
    static void initCurses();
    static void flush();
    static void drawLevel(const Level &level, const Player &player);
    static void drawMainMenu(int highlighted, Difficulty *difficulty);
    static void drawPauseMenu(int highlighted);
    static void drawInventory(int highlighted);
    static void terminate();
};
class ArrowDisplay {
  private:
    std::vector<std::string> options;
    size_t selected = 0;
    int x, y; // Position of the menu

  public:
    ArrowDisplay(const std::vector<std::string> &opts, int pos_x = 0,
                 int pos_y = 0)
        : options(opts), x(pos_x), y(pos_y) {}

    void display();
    int run();
};
