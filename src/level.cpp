#include "include/level.h"
#include "include/enums.h"
#include "include/vector2d.h"
#include <stdlib.h>
#include <unistd.h>
#include <vector>

/*
 * Constructor for the Level class
 * Generates a maze using Prim's algorithm, places the exit, and spawns items.
 *
 * @param size       Size of the square maze (size x size)
 * @param startPos   Starting position for the maze generation and player
 * @param itemCount  Number of items to randomly place in the maze
 * @return none
 */
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

/*
 * Selects and marks an exit point in the maze
 * Filters out tiles that are too close to the start, then randomly chooses
 * one of the remaining path tiles to serve as the exit.
 *
 * @return void
 */
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
                          TileObject::Chest, TileObject::Pickaxe};
    int weights[] = {50, 25, 10, 15};

    std::vector<Vector2D> pathList = std::vector<Vector2D>();
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            if (this->maze[i][j] == TileObject::None && i > 0 && j > 0)
                pathList.push_back(Vector2D(i, j));
    
    std::vector<Vector2D> selectedTiles = getRandPaths(pathList, count);

    // Fixed 1 of each of collectable
    for(int i = 0; i < 4; i++) {
        Vector2D pos = selectedTiles[i];
        this->maze[pos.y][pos.x] = items[i];
    }
    selectedTiles.erase(selectedTiles.begin(), selectedTiles.begin() + 4);
    if (count <= 4)
        return;
        
    // Weighted randomization after placing 1 of each collectable 
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

/*
 * Gets the current size of the level
 *
 * @return int The size of the square maze
 */
int Level::getSize() const { return this->size; }

/*
 * Gets the current state of the maze
 *
 * @return TileMap 2D vector representing maze tiles
 */
TileMap Level::getMaze() const { return this->maze; }

/*
 * Gets the player's starting position in the maze
 *
 * @return Vector2D Starting coordinates
 */
Vector2D Level::getStart() const { return this->startPos; }

/*
 * Gets the exit position of the maze
 *
 * @return Vector2D Exit coordinates
 */
Vector2D Level::getEnd() const { return this->endPos; }

/*
 * Gets the current game status from the level
 *
 * @return bool Whether the game is ongoing or not
 */
bool Level::getGameStatus() const { return this->gameStatus; }

/*
 * Checks if a given move is valid (not out of bounds or a wall)
 *
 * @param playerPos The new position the player wants to move to
 * @return bool True if move is valid, false otherwise
 */
bool Level::isValidMove(Vector2D playerPos) const {
    if (playerPos.x < 0 || playerPos.x >= this->size || playerPos.y < 0 ||
        playerPos.y >= this->size)
        return false;

    if (this->maze[playerPos.y][playerPos.x] == TileObject::Wall)
        return false;

    return true;
}

/*
 * Sets the tile at a specific position to the given tile object
 *
 * @param pos    Position of the tile
 * @param target TileObject to place at the position
 * @return void
 */
void Level::setTile(Vector2D pos, TileObject target) {
    this->maze[pos.y][pos.x] = target;
}

/*
 * Gets the tile object at a specific position in the maze
 *
 * @param pos Position of the tile to retrieve
 * @return TileObject The tile object at the specified position
 */
TileObject Level::getTile(Vector2D pos) const {
    if (pos.x < 0 || pos.x >= this->size || pos.y < 0 || pos.y >= this->size)
        return TileObject::None;

    return this->maze[pos.y][pos.x];
}
