#pragma once
#include <Novice.h>
#include "Data.h"

class Player{
public:

/// <summary>
/// Function for moving the player
/// </summary>
/// <param name="player">Player's values</param>
/// <param name="keys">Key input values</param>
	void Move(Player_& player,Line& line, const char* keys,const char* preKeys);

	/// <summary>
	/// Function for drawing the player
	/// </summary>
	/// <param name="player">Player's values</param>
	void Draw(Player_& player);


};


