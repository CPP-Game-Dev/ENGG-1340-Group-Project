#pragma once

#include "vector2d.h"


/*
* Class to store player-related functions & player data
*/
class Player
{
private:
	#pragma region Class Variables
	int stamina;	//Current stamina
	int staminaMax;	//Maximum stamina
	int fov;		//Radius of field-of-view
	Vector2D pos;	//Player position
	#pragma endregion
public:
	
	Player();													//Initialize a new Player object (used for new game)
	Player(Vector2D pos, int stamina, int staminaMax, int fov);	//Create a Player object with predetermined stats (used to load a save)

};