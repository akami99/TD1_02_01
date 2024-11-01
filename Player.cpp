#include <Novice.h>
#include "Data.h"
#include "Player.h"

const float leftWall = 0.0f;
const float rightWall = 1280.0f;

void Player::Move(Player_& player, Line& line, const char* keys, const char* preKeys) {
	if (keys[DIK_D] || keys[DIK_RIGHT]) {
		player.pos.x += player.speed;
	}

	if (keys[DIK_A] || keys[DIK_LEFT]) {
		player.pos.x -= player.speed;
	}

	if (player.pos.x <leftWall) {
		player.pos.x = leftWall;
	}

	if (player.pos.x > rightWall - player.radius) {
		player.pos.x = rightWall-player.radius;
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

	if (!keys[DIK_W] && !preKeys[DIK_W] && !keys[DIK_UP] && !preKeys[DIK_UP]) {
		player.isJanp = false;
	}

	//player no sokudo ni kasokudo wo tasu
	player.velocity.x += player.acceleration.x;
	player.velocity.y -= player.acceleration.y;

	//player no iti ni sokudo wo tasu
	player.pos.x += player.velocity.x;
	player.pos.y -= player.velocity.y;

	if (player.pos.y >= line.pos.y - player.radius) {
		player.pos.y = line.pos.y - player.radius;
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

void Player::Draw(Player_& player) {
	Novice::DrawBox(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y),
		static_cast<int>(player.radius), static_cast<int>(player.radius), 0.0f, WHITE, kFillModeSolid);
}