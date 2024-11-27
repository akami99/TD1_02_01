#include <Novice.h>
#include "Data.h"
#include "Player.h"
#include "Tool.h"

const float leftWall = 0.0f;
const float rightWall = 1280.0f;

void Player::Initialize(Player_& player, Flash_& flash) {
	player.pos = { 32.0f,600.0f };
	player.direction = { 1.0f, 0.0f };
	player.velocity = { 0.0f, 0.0f };
	player.hp = 300;
	player.energy = 600;
	player.isFlash = false;
	player.isHighFlash = false;
	player.isJanp = false;
	player.janpNum = 0;
	player.speed = 2.0f;
	player.isJanpSpeed = false;
	player.isHit = false;

	flash.pos = { 0.0f,0.0f };
	flash.direction = { 0.0f, 0.0f };
}

void Player::Move(Player_& player, Line& line, const char* keys, const char* preKeys) {
	player.direction = { 0.0 };

	if (player.isFlash) {
		player.isFlash = false;
	} else if (player.isHighFlash) {
		player.isHighFlash = false;
	}

	if (player.flashTime > 0) {
		player.flashTime--;
	}
	if (player.hightFlashTime > 0) {
		player.hightFlashTime--;
	}

	if (keys[DIK_D] || keys[DIK_RIGHT]) {
		player.pos.x += player.speed;
		player.direction.x = 1;
		player.isLeft = false;
		player.isRight = true;

	}

	if (keys[DIK_A] || keys[DIK_LEFT]) {
		player.pos.x -= player.speed;

		if (player.direction.x != 0) {
			player.direction.x = 0;
		} else {
			player.direction.x = -1;
		}

		player.isRight = false;
		player.isLeft = true;

	}

	if (keys[DIK_S] || keys[DIK_DOWN]) {
		player.direction.y = 1;
	}

	if (keys[DIK_W] || keys[DIK_UP]) {
		if (player.direction.y != 0) {
			player.direction.y = 0;
		} else {
			player.direction.y = -1;
		}
	}

	if (player.pos.x < leftWall) {
		player.pos.x = leftWall;
	}

	if (player.pos.x > rightWall - player.radius * 2) {
		player.pos.x = rightWall - player.radius * 2;
	}

	//jump
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		if (!player.isJanp) {
			if (player.janpNum < 2) {
				player.velocity.y = 10.0f;
				player.janpNum++;
				player.isJanp = true;
				player.isJanpSpeed = true;
				//player.janpingFrame = 10;
			}
		}
	}

	if (!keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		player.isJanp = false;
	}

	//プレイヤーの速度に加速度を足す
	player.velocity.x += player.acceleration.x;
	player.velocity.y -= player.acceleration.y;

	//プレイヤーの位置に速度を足す
	player.pos.x += player.velocity.x;
	player.pos.y -= player.velocity.y;

	if (player.pos.y >= line.pos.y - player.radius * 2) {
		player.pos.y = line.pos.y - player.radius * 2;
		player.isJanp = false;
		player.isJanpSpeed = false;
		player.janpNum = 0;
	}

	if (player.isJanpSpeed) {
		player.speed = 5.0f;
	} else {
		player.speed = 8.0f;
	}

}

void Player::Attack(Player_& player, Boss_& boss, Flash_& flash, const char* keys, const char* preKeys) {
	// 通常ライト
	if (keys[DIK_J] && !preKeys[DIK_J]) {
		if (!player.isFlash) {
			player.isFlash = true;
			player.isHighFlash = false;
			player.hightFlashTime = 0;
			player.flashTime = 10;
		} else {
			player.isFlash = false;
			player.flashTime = 0;
		}
	}
	// 強化ライト
	if (keys[DIK_K] && !preKeys[DIK_K]) {
		if (!player.isHighFlash) {
			player.isHighFlash = true;
			player.isFlash = false;
			player.flashTime = 0;
			player.hightFlashTime = 10;
		} else {
			player.isHighFlash = false;
			player.hightFlashTime = 0;
		}
	}

	if (player.isFlash) {
		player.energy -= 25;
	} else if (player.isHighFlash) {
		player.energy -= 50;
	}

	if (player.energy <= 0) {
		player.isFlash = false;
	}

	if (player.energy < player.kMaxEnergy) {
		player.energy++;
	}

	//フラッシュライトの向きを最後にプレイヤーが向いていた方向にする
	if (player.direction.x != 0 || player.direction.y != 0) {
		if (player.direction.y != 0) { // 上が優先されるので左右情報は無くしておく
			flash.direction.x = 0;
			flash.direction.y = player.direction.y;
		} else {
			flash.direction = player.direction;
		}
	}
	//フラッシュライトの位置を現在のプレイヤーのライトの位置にする
	flash.pos = { player.pos.x + player.radius + player.radius / 3 * flash.direction.x, player.pos.y + player.radius + player.radius / 2 * flash.direction.y };
	FlashHitBox(player, boss, flash);
}

void Player::Draw(Player_& player, Flash_& flash) {
	if (flash.direction.x > 0) {         //右
		Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), player.imageRight, 1, 1, 0.0f, WHITE);
	} else if (flash.direction.x < 0) {  //左
		Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), player.imageLeft, 1, 1, 0.0f, WHITE);
	} else if (flash.direction.y > 0) {  //下
		Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), player.imageDown, 1, 1, 0.0f, WHITE);
	} else if (flash.direction.y < 0) {  //上
		Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), player.imageUp, 1, 1, 0.0f, WHITE);
	} else {                             //最初
		Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), player.imageRight, 1, 1, 0.0f, WHITE);
	}
}

void Player::DrawFlash(Player_& player, Flash_& flash) {
	// 通常ライト
	if (player.flashTime > 0) {
		if (flash.direction.y > 0) {                                   //下
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.width), static_cast<int>(flash.pos.y + flash.range),
				static_cast<int>(flash.pos.x - flash.width), static_cast<int>(flash.pos.y + flash.range),
				0xe0dd8780, kFillModeSolid);
		} else if (flash.direction.y < 0) {                            //上
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.width), static_cast<int>(flash.pos.y - flash.range),
				static_cast<int>(flash.pos.x - flash.width), static_cast<int>(flash.pos.y - flash.range),
				0xe0dd8780, kFillModeSolid);
		} else if (flash.direction.x > 0) {                            //右
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.range), static_cast<int>(flash.pos.y + flash.width),
				static_cast<int>(flash.pos.x + flash.range), static_cast<int>(flash.pos.y - flash.width),
				0xe0dd8780, kFillModeSolid);
		} else if (flash.direction.x < 0) {                            //左
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x - flash.range), static_cast<int>(flash.pos.y + flash.width),
				static_cast<int>(flash.pos.x - flash.range), static_cast<int>(flash.pos.y - flash.width),
				0xe0dd8780, kFillModeSolid);
		}
	}
	// 強化ライト
	if (player.hightFlashTime > 0) {
		if (flash.direction.y > 0) {                                   //下
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highWidth), static_cast<int>(flash.pos.y + flash.highRange),
				static_cast<int>(flash.pos.x - flash.highWidth), static_cast<int>(flash.pos.y + flash.highRange),
				0xe0dd87d0, kFillModeSolid);
		} else if (flash.direction.y < 0) {                            //上
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highWidth), static_cast<int>(flash.pos.y - flash.highRange),
				static_cast<int>(flash.pos.x - flash.highWidth), static_cast<int>(flash.pos.y - flash.highRange),
				0xe0dd87d0, kFillModeSolid);
		} else if (flash.direction.x > 0) {                            //右
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highRange), static_cast<int>(flash.pos.y + flash.highWidth),
				static_cast<int>(flash.pos.x + flash.highRange), static_cast<int>(flash.pos.y - flash.highWidth),
				0xe0dd87d0, kFillModeSolid);
		} else if (flash.direction.x < 0) {                            //左
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x - flash.highRange), static_cast<int>(flash.pos.y + flash.highWidth),
				static_cast<int>(flash.pos.x - flash.highRange), static_cast<int>(flash.pos.y - flash.highWidth),
				0xe0dd87d0, kFillModeSolid);
		}
	}
}

//=========================
//当たり判定の作成 　　　　　　　　　　　
//=========================
//---------------------------------------------------------------------------------------------

//通常ライト
void Player::FlashHitBox(Player_& player, Boss_& boss, Flash_& flash) {
	boss.top = { boss.pos.x + boss.size.x / 2, boss.pos.y + boss.size.y / 2 };
	boss.bottom = { boss.pos.x + boss.size.x / 2, boss.pos.y + boss.size.y * 3 / 4 };

	// ライトと敵が重なっているかチェック
	// 通常ライト
	if (player.isFlash) {
		// ライトの上下の当たり判定
		if (boss.size.x / 2.0f + flash.range >=
			Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y))) {
			boss.isHitTop = true;
		} else {
			boss.isHitTop = false;
		}

		if (flash.direction.y > 0) {                                   //下
			// 外積を使ったライトの衝突判定
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x + flash.width, flash.pos.x) / flash.range,
				Measurement(flash.pos.y + flash.range, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x + flash.width, flash.pos.x) / flash.range,
					Measurement(flash.pos.y + flash.range, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x - flash.width, flash.pos.x) / flash.range,
				Measurement(flash.pos.y + flash.range, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x - flash.width, flash.pos.x) / flash.range,
					Measurement(flash.pos.y + flash.range, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.y < 0) {                            //上
			// 外積を使ったライトの衝突判定 
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x + flash.width, flash.pos.x) / flash.range,
				Measurement(flash.pos.y - flash.range, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x + flash.width, flash.pos.x) / flash.range,
					Measurement(flash.pos.y - flash.range, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x - flash.width, flash.pos.x) / flash.range,
				Measurement(flash.pos.y - flash.range, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x - flash.width, flash.pos.x) / flash.range,
					Measurement(flash.pos.y - flash.range, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.x > 0) {                            //右
			// 外積を使ったライトの衝突判定
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x + flash.range, flash.pos.x) / flash.range,
				Measurement(flash.pos.y + flash.width, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x + flash.range, flash.pos.x) / flash.range,
					Measurement(flash.pos.y + flash.width, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x + flash.range, flash.pos.x) / flash.range,
				Measurement(flash.pos.y - flash.width, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x + flash.range, flash.pos.x) / flash.range,
					Measurement(flash.pos.y - flash.width, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.x < 0) {                            //左
			// 外積を使ったライトの衝突判定
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x - flash.range, flash.pos.x) / flash.range,
				Measurement(flash.pos.y + flash.width, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x - flash.range, flash.pos.x) / flash.range,
					Measurement(flash.pos.y + flash.width, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x - flash.range, flash.pos.x) / flash.range,
				Measurement(flash.pos.y - flash.width, flash.pos.y) / flash.range,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x - flash.range, flash.pos.x) / flash.range,
					Measurement(flash.pos.y - flash.width, flash.pos.y) / flash.range,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		}
	}
	// 強化ライト
	if (player.isHighFlash) {
		// ライトの上下の当たり判定
		if (boss.size.x / 2.0f + flash.highRange >=
			Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y))) {
			boss.isHitTop = true;
		} else {
			boss.isHitTop = false;
		}

		if (flash.direction.y > 0) {                                   //下
			// 外積を使ったライトの衝突判定
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x + flash.highWidth, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y + flash.highRange, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x + flash.highWidth, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y + flash.highRange, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x - flash.highWidth, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y + flash.highRange, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x - flash.highWidth, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y + flash.highRange, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.y < 0) {                            //上
			// 外積を使ったライトの衝突判定                 
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x + flash.highWidth, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y - flash.highRange, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x + flash.highWidth, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y - flash.highRange, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x - flash.highWidth, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y - flash.highRange, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x - flash.highWidth, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y - flash.highRange, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.x > 0) {                            //右
			// 外積を使ったライトの衝突判定
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x + flash.highRange, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y + flash.highWidth, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x + flash.highRange, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y + flash.highWidth, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x + flash.highRange, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y - flash.highWidth, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x + flash.highRange, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y - flash.highWidth, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		} else if (flash.direction.x < 0) {                            //左
			// 外積を使ったライトの衝突判定
			if (0 < CrossProduct(     // + で範囲の中
				Measurement(flash.pos.x - flash.highRange, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y + flash.highWidth, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 < CrossProduct(     // + で範囲の中
					Measurement(flash.pos.x - flash.highRange, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y + flash.highWidth, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitRight = true;
			} else {
				boss.isHitRight = false;
			}
			if (0 > CrossProduct(     // - で範囲の中
				Measurement(flash.pos.x - flash.highRange, flash.pos.x) / flash.highRange,
				Measurement(flash.pos.y - flash.highWidth, flash.pos.y) / flash.highRange,
				Measurement(boss.top.x, flash.pos.x) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)),
				Measurement(boss.top.y, flash.pos.y) / Length(Measurement(boss.top.x, flash.pos.x), Measurement(boss.top.y, flash.pos.y)))
				|| 0 > CrossProduct(     // - で範囲の中
					Measurement(flash.pos.x - flash.highRange, flash.pos.x) / flash.highRange,
					Measurement(flash.pos.y - flash.highWidth, flash.pos.y) / flash.highRange,
					Measurement(boss.bottom.x, flash.pos.x) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)),
					Measurement(boss.bottom.y, flash.pos.y) / Length(Measurement(boss.bottom.x, flash.pos.x), Measurement(boss.bottom.y, flash.pos.y)))
				) {
				boss.isHitLeft = true;
			} else {
				boss.isHitLeft = false;
			}
		}
	}

	// 最終的な判定
	if (boss.isHitTop && boss.isHitRight && boss.isHitLeft) {
		if (player.isFlash) {
			if (boss.hp > 0) { // 敵へダメージ
				boss.hitStopTime = 4;
				boss.hp--;
				player.isFlash = false;
			}
		} else if (player.isHighFlash) {
			if (boss.hp > 0) { // 敵へダメージ
				boss.hitStopTime = 5;
				boss.hp -= 2;
				player.isHighFlash = false;
			}
		}
	}
}
//---------------------------------------------------------------------------------------------

//タイトルやクリア時の画面の描画
void Player::DrawBackGround(int backGround) {
	Novice::DrawSprite(0, 0, backGround, 1, 1, 0.0f, WHITE);
}

//背景の描画
void Player::DrawBackGround(Line& backGround, Shake shake) {
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK, kFillModeSolid);
	Novice::DrawSprite(static_cast<int>(0 + shake.pos.x + shake.bgPos.x), static_cast<int>(0 + shake.pos.y + shake.bgPos.y), backGround.backGround, 1, 1, 0.0f, WHITE);
}