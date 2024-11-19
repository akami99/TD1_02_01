#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Data.h"
#include "Ui.h"

void Ui::DrawHpBar(const int& posX, const int& posY, const int& hp, const int& type) {
	
	if (type == 0) { // 敵
		Novice::DrawBox
		(
			posX + 600, posY + 40,
			600, 40,
			static_cast<float>(M_PI),
			BLACK, kFillModeSolid
		);

		Novice::DrawBox
		(
			posX, posY,
			hp * 3, 40,
			0.0f,
			0xff00ffff, kFillModeSolid
		);
	} 
	//else {         // プレイヤー
	//	Novice::DrawBox
	//	(
	//		posX, posY,
	//		240, 70,
	//		0.0f,
	//		GREEN, kFillModeSolid
	//	);
	//}

	
}