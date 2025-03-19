#pragma once

#include<vector>
#include"include/Enums.h"

/*
Class to store the current level's maze layout
has functions to generate the maze
*/
class Level
{
private:
	std::vector<std::vector<TileObject>> maze;
public:
	Level(int mazeSize, int itemCount);

	void initMaze(int size);		//Set the maze vector to be all TileObject.Wall
	void generateMaze(int size);	//Generate the maze using iterative randomized Prim's algorithm
	void placeItems(int itemCount);	//Place items randomly in the maze
};