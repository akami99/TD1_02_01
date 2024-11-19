#pragma once
#include <Novice.h>
#include "Data.h"

class Ui {
public:

	/// <summary>
	/// HPゲージの描画関数
	/// </summary>
	/// <param name="posX">描画位置X</param>
	/// <param name="posY">描画位置Y</param>
	/// <param name="hp">対象のHP</param>
	/// <param name="type">0 = 敵, 1 = プレイヤー</param>
	void DrawHpBar(const int& posX, const int& posY, const int& hp, const int& type);
};