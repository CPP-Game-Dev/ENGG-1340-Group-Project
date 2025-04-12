#pragma once
#include "enums.h"
#include "vector2d.h"
#include <vector>

/*
 * Class to store the current level's maze layout
 * has functions to generate the maze
 */
class Level {
  private:
    TileMap maze;
    std::vector<Vector2D> wallList;
    Vector2D startPos;
    Vector2D endPos;
    int itemCount;
    int size;

    void generateMaze(Vector2D pos);
    void getAdjWalls(Vector2D pos);
    bool verifyWall(Vector2D wallPos);
    void placeItems(int itemCount);
    void setExit();

    void print();

  public:
    Level(int size, Vector2D startPos, int itemCount);
    int getSize() const;
    TileMap getMaze() const;
    Vector2D getStart() const;
    Vector2D getEnd() const;
};
