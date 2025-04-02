#include "include/level.h"
#include "include/enums.h"
#include "include/vector2d.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

Level::Level(int size, Vector2D startPos, int itemCount) {
    this->size = size;
    this->maze = std::vector<std::vector<TileObject>>(
        size, std::vector<TileObject>(size, Wall));
    this->wallList = std::vector<Vector2D>();
    this->startPos = startPos;
    this->itemCount = itemCount;
    generateMaze(startPos);
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
    this->maze[pos.y][pos.x] = None;
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
            this->maze[pos.y][pos.x] = None;
            getAdjWalls(pos);
        }

        // Remove any checked wall from the list of unchecked walls
        wallList.erase(wallList.begin() + index);
    }

    // Set starting position to player
    this->maze[startPos.y][startPos.x] = Player;
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
    if (pos.y > 0 && this->maze[pos.y - 1][pos.x] == Wall)
        this->wallList.push_back(Vector2D(pos.y - 1, pos.x));

    // Check left
    if (pos.x > 0 && this->maze[pos.y][pos.x - 1] == Wall)
        this->wallList.push_back(Vector2D(pos.y, pos.x - 1));

    // Check below
    if (pos.y < this->size - 1 && this->maze[pos.y + 1][pos.x] == Wall)
        this->wallList.push_back(Vector2D(pos.y + 1, pos.x));

    // Check right
    if (pos.x < this->size - 1 && this->maze[pos.y][pos.x + 1] == Wall)
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
    if (wallPos.y > 0 && this->maze[wallPos.y - 1][wallPos.x] == None)
        count++;

    // Check left
    if (wallPos.x > 0 && this->maze[wallPos.y][wallPos.x - 1] == None)
        count++;

    // Check down
    if (wallPos.y < this->size - 1 &&
        this->maze[wallPos.y + 1][wallPos.x] == None)
        count++;

    // Check right
    if (wallPos.x < this->size - 1 &&
        this->maze[wallPos.y][wallPos.x + 1] == None)
        count++;

    return count == 1;
}

/*
 * Function to put a given amount of random items in random spots of the maze
 *
 * Args: number of items to put
 */
void Level::placeItems(int itemCount) {
    // Make temporary variable to store a list of useful
    std::vector<Vector2D> tileList =
        std::vector<Vector2D>(this->size * this->size);
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            if (this->maze[i][j] == None)
                tileList.push_back(Vector2D(i, j));

#pragma region Weighted Randomization
    // Hard-coded weights & respective items
    int weights[6] = { 45, 20, 5, 10, 15, 5};
    TileObject items[6] = {TileObject::Ration,  TileObject::EnergyDrink,
                           TileObject::Battery, TileObject::Pickaxe,
                           TileObject::Chest,  TileObject::Mimic};

    // Calculate sum of weights
    int weightSum = 0;
    for (int i = 0; i < 6; i++)
        weightSum += weights[i];

    // Place a given amount of items
    for (int i = 0; i < itemCount; i++) {
        int rnd = int(std::rand() % weightSum);
        for (int j = 0; j < 6; j++) {
            if (rnd < weights[j]) {
                // Get random empty tile
                Vector2D tilePos =
                    tileList.at(int(std::rand() % tileList.size()));
                this->maze[tilePos.y][tilePos.x] = items[j];
                continue;
            }
            rnd -= weights[j];
        }
    }
#pragma endregion
}

void Level::setEndpoint() {
    for (int i = this->size - 1; i >= 0; i--) {
        for (int j = this->size - 1; j >= 0; j--) {
            if (this->maze[i][j] != Wall && this->maze[i][j] != Player) {
                this->endPos = Vector2D(i, j);
                return;
            }
        }
    }
}
int Level::getSize() const { return this->size; }

std::vector<std::vector<TileObject>> Level::getMaze() const { return this->maze; }

Vector2D Level::getStart() const { return this->startPos; }

Vector2D Level::getEnd() const { return this->endPos; }