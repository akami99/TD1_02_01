#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Data.h"
#include "Ui.h"

void Ui::DrawGauge(const int& posX, const int& posY, const int& point, const int& type) {
	//数値メモ::playerHP=300; playerEN=600; bossHP=200;
	if (type == 0) { // 敵
		// 黒
		Novice::DrawBox
		(
			posX + 600, posY + 40,
			600, 40,
			static_cast<float>(M_PI),
			BLACK, kFillModeSolid
		);
		// 赤
		Novice::DrawBox
		(
			posX, posY,
			point * 3, 40,
			0.0f,
			RED, kFillModeSolid
		);
	} else if (type == 1) { // プレイヤー
		// 黒
		Novice::DrawBox
		(
			posX + 300, posY + 30,
			300, 30,
			static_cast<float>(M_PI),
			BLACK, kFillModeSolid
		);
		// 緑
		Novice::DrawBox
		(
			posX, posY,
			point, 30,
			0.0f,
			GREEN, kFillModeSolid
		);
	} else if (type == 2) { // エネルギー
		// 黒
		Novice::DrawBox
		(
			posX + 300, posY + 30,
			300, 30,
			static_cast<float>(M_PI),
			BLACK, kFillModeSolid
		);
		// 青
		Novice::DrawBox
		(
			posX, posY,
			static_cast<int>(point / 2), 30,
			0.0f,
			BLUE, kFillModeSolid
		);
	}
}