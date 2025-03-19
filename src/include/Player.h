#pragma once

/*
Class to store player-related functions & player data
*/
class Player
{
private:
	#pragma region Class Variables
	int stamina;	//Current stamina
	int staminaMax;	//Maximum stamina
	int xPos;		//X position, right is positive
	int yPos;		//Y position, down is positive
	int fov;		//Radius of field-of-view
	#pragma endregion
public:
	
	Player();										//Initialize a new Player object (used for new game)
	Player(int xPos, int yPos, 
		   int stamina, int staminaMax, int fov);	//Create a Player object with predetermined stats (used to load a save)


	/*
	Deconstrucutor

	Frees any dynamically allocated memory within the class and delete the object
	*/
	~Player();
};