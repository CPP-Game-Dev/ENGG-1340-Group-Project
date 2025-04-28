#include "include/level.h"
#include "include/enums.h"
#include "include/utils.h"
#include "include/vector2d.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

Level::Level(int size, Vector2D startPos, int itemCount) {
    this->size = size;
    this->maze = TileMap(size, std::vector<TileObject>(size, TileObject::Wall));
    this->wallList = std::vector<Vector2D>();
    this->startPos = startPos;
    this->itemCount = itemCount;
    // Run the generation algorithm
    generateMaze(startPos);
    // Set starting position to player
    // this->maze[startPos.y][startPos.x] = TileObject::Player;
    setExit();
    placeItems(itemCount);
}

/*
 * Function to generate a maze of pre-determined size with given starting point
 * Uses Iterative Randomized Prim's Algorithm
 *
 * Args: starting position
 */
void Level::generateMaze(Vector2D pos) {
#pragma region Starting Cell
    this->maze[pos.y][pos.x] = TileObject::None;
    getAdjWalls(pos);
#pragma endregion

    // While there are walls to be checked/removed, do Prim's algorithm
    while (this->wallList.size() > 0 || pos == this->startPos) {
        // Get random wall to verify
        int index = int(rand() % this->wallList.size());
        pos = this->wallList[index];

        // Check if chosen wall is connected to too many empty tiles
        if (verifyWall(pos)) {
            // Remove wall
            this->maze[pos.y][pos.x] = TileObject::None;
            getAdjWalls(pos);
        }

        // Remove any checked wall from the list of unchecked walls
        wallList.erase(wallList.begin() + index);
    }
}

/*
 * Function to find all walls next to given position
 * Any wall found will be added to the list of walls
 *
 * Args: position to check
 *
 */
void Level::getAdjWalls(Vector2D pos) {
    // Check above
    if (pos.y > 0 && this->maze[pos.y - 1][pos.x] == TileObject::Wall)
        this->wallList.push_back(Vector2D(pos.y - 1, pos.x));

    // Check left
    if (pos.x > 0 && this->maze[pos.y][pos.x - 1] == TileObject::Wall)
        this->wallList.push_back(Vector2D(pos.y, pos.x - 1));

    // Check below
    if (pos.y < this->size - 1 &&
        this->maze[pos.y + 1][pos.x] == TileObject::Wall)
        this->wallList.push_back(Vector2D(pos.y + 1, pos.x));

    // Check right
    if (pos.x < this->size - 1 &&
        this->maze[pos.y][pos.x + 1] == TileObject::Wall)
        this->wallList.push_back(Vector2D(pos.y, pos.x + 1));
}

/*
 * Function to verify if a wall can be removed
 * A wall can only be removed if it is connected to no more than 1 empty tiles
 *
 * Assumption: only walls next to at least one empty tile can exist in the list
 * of walls
 *
 * Args: position of wall to verify
 *
 * Return: if the wall can be removed or not
 */
bool Level::verifyWall(Vector2D wallPos) {
    int count = 0;
    // Check up
    if (wallPos.y > 0 &&
        this->maze[wallPos.y - 1][wallPos.x] == TileObject::None)
        count++;

    // Check left
    if (wallPos.x > 0 &&
        this->maze[wallPos.y][wallPos.x - 1] == TileObject::None)
        count++;

    // Check down
    if (wallPos.y < this->size - 1 &&
        this->maze[wallPos.y + 1][wallPos.x] == TileObject::None)
        count++;

    // Check right
    if (wallPos.x < this->size - 1 &&
        this->maze[wallPos.y][wallPos.x + 1] == TileObject::None)
        count++;

    return count == 1;
}

void Level::setExit() {
    std::vector<Vector2D> tileList = std::vector<Vector2D>();
    Vector2D pos;
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            pos = Vector2D(i, j);
            if (this->maze[i][j] == TileObject::None) {
                if (std::abs(startPos.y - i) + std::abs(startPos.x - j) <
                    this->size)
                    continue;
                tileList.push_back(pos);
            }
        }
    }
    pos = tileList[int(rand() % tileList.size())];
    this->maze[pos.y][pos.x] = TileObject::Exit;
    this->endPos = pos;
}

#pragma region RNG Helper
std::vector<Vector2D> getRandPaths(std::vector<Vector2D> &pathList, int count) {
    std::vector<Vector2D> selectedTiles;
    int index;
    for (int i = 0; i < count; i++) {
        index = rand() % (int)pathList.size();
        selectedTiles.push_back(pathList[index]);
        pathList.erase(pathList.begin() + index);
    }
    return selectedTiles;
}
#pragma endregion

/*
 * Function to put a given amount of random items in random spots of the maze
 *
 * Args: number of items to put
 */
void Level::placeItems(int count) {
    TileObject items[] = {TileObject::Ration, TileObject::EnergyDrink,
                          TileObject::Battery, TileObject::Chest};
    int weights[] = {66, 22, 2, 10};

    std::vector<Vector2D> pathList = std::vector<Vector2D>();
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            if (this->maze[i][j] == TileObject::None)
                pathList.push_back(Vector2D(i, j));
    std::vector<Vector2D> selectedTiles = getRandPaths(pathList, count);
    for (auto pos : selectedTiles) {
        TileObject item = TileObject::None;
        int rnd = rand() % 100;
        for (int i = 0; i < 4; i++) {
            rnd -= weights[i];
            if (rnd <= 0) {
                item = items[i];
                break;
            }
        }
        this->maze[pos.y][pos.x] = item;
    }
}

int Level::getSize() const { return this->size; }

TileMap Level::getMaze() const { return this->maze; }

Vector2D Level::getStart() const { return this->startPos; }

Vector2D Level::getEnd() const { return this->endPos; }

bool Level::getGameStatus() const { return this->gameStatus; }

bool Level::isValidMove(Vector2D playerPos) const {
    if (playerPos.x < 0 || playerPos.x >= this->size || playerPos.y < 0 ||
        playerPos.y >= this->size)
        return false;

    if (this->maze[playerPos.y][playerPos.x] == TileObject::Wall)
        return false;

    return true;
}

void Level::setTile(Vector2D pos, TileObject target) {
    this->maze[pos.y][pos.x] = target;
}

TileObject Level::getTile(Vector2D pos) const {
    if (pos.x < 0 || pos.x >= this->size || pos.y < 0 || pos.y >= this->size)
        return TileObject::None;

    return this->maze[pos.y][pos.x];
}
