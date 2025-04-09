#pragma once

#include "level.h"

/*
 * Class to store functions to draw the game on the console
 */
class Display {
  public:
    static void initCurses();
    static void flush();
    static void drawLevel(Level level);
    static void drawMainMenu(int highlighted);
    static void drawPauseMenu(int highlighted);
    static void drawInventory(int highlighted); 
    static void terminate();
};
