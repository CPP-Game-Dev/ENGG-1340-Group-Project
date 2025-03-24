#pragma once

/*
File to store all enums used in other classes
*/

//Enum for different tile objects (anything that appears on the map is a tile object)
enum TileObject
{
	Player, Wall, None,							//Player, Wall of maze, Path
	RegenSmall, RegenMedium, RegenBig,			//3 tiers of instant Stamina replenish
	StaminaSmall, StaminaMedium, StaminaBig,	//3 tiers of maximum Stamina boost
	VisionSmall, VisionMedium, VisionBig,		//3 tiers of vision range boost
};

//Enum for the 3 levels of difficulty (to be implemented after MVP)
enum Difficulty
{
	Catacombs, Labyrinth, Purgatory		//Easy, Medium, Hard :3
};