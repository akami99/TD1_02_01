#pragma once
#pragma once

#include "Data.h"
#include "Player.h"
#include "boss.h"
#include "Ui.h"

// ゲーム全体の初期化を行う関数
void InitializeGame(Player_& player, Flash_& flash, Boss_& boss, Line& line);
