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
	/// Function for attack the player
	/// </summary>
	/// <param name="player">Player's values</param>
	/// <param name="flash">Flash's values</param>
	/// <param name="keys">Key input values</param>
	void Attack(Player_& player, Flash_& flash, const char* keys, const char* preKeys);

	/// <summary>
	/// Function for drawing the player
	/// </summary>
	/// <param name="player">Player's values</param>
	void Draw(Player_& player);

	/// <summary>
	/// Function for drawing the flash
	/// </summary>
	/// <param name="isFlash">isFlash flag</param>
	/// <param name="flash">Flash's values</param>
	void DrawFlash(Player_& player, Flash_& flash);

	void DrawBackGround(Line& backGround, Shake shake);
};


