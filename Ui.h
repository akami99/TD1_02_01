#pragma once
#include <Novice.h>
#include "Data.h"

class Ui {
public:

	/// <summary>
	/// リソース系ゲージの描画関数
	/// </summary>
	/// <param name="posX">描画位置X</param>
	/// <param name="posY">描画位置Y</param>
	/// <param name="point">対象の値</param>
	/// <param name="type">0 = 敵, 1 = プレイヤー, 2 = エネルギー</param>
	void DrawGauge(const int& posX, const int& posY, const int& point, const int& type);

	/// <summary>
	/// 文字画像の描画
	/// </summary>
	/// <param name="posX">描画位置X</param>
	/// <param name="posY">描画位置Y</param>
	/// <param name="font">文字画像</param>
	void DrawFont(int posX, int posY, int font);

	/// <summary>
	/// 薄い文字画像の描画
	/// </summary>
	/// <param name="posX">描画位置X</param>
	/// <param name="posY">描画位置Y</param>
	/// <param name="font">文字画像</param>
	void DrawLightFont(int posX, int posY, int font);
};