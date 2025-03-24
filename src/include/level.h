#pragma once
#include <vector>
#include "enums.h"
#include "vector2d.h"

/*
* Class to store the current level's maze layout
* has functions to generate the maze
*/
class Level
{
private:
	std::vector<std::vector<TileObject>> maze;
	std::vector<Vector2D> wallList;
	Vector2D startPos;
	Vector2D endPos;
	int itemCount;
public:
	Level(int size, Vector2D startPos, int itemCount);
};