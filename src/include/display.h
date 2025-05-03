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
    static void drawLevel(const Level &level, const Player &player, int currentLevel);
    static void drawMainMenu(int highlighted);
    static void drawDifficultyMenu(int highlighted);
    static void drawPauseMenu(int highlighted);
    static void drawHelpMenu(int highlighted);
    static void drawInventoryMenu(int highlighted, const std::vector<std::unique_ptr<Item> > &inventory);
    static void drawItemMenu(int highlighted, std::string desc);
    static void drawGameOverMenu(int highlighted);
    static void terminate();
};