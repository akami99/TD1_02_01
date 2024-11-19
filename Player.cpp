﻿#include <Novice.h>
#include "Data.h"
#include "Player.h"

const float leftWall = 0.0f;
const float rightWall = 1280.0f;

void Player::Move(Player_& player, Line& line, const char* keys, const char* preKeys) {
	player.direction = { 0.0 };

	if (keys[DIK_D] || keys[DIK_RIGHT]) {
		player.pos.x += player.speed;
		player.direction.x = 1;
	}

	if (keys[DIK_A] || keys[DIK_LEFT]) {
		player.pos.x -= player.speed;

		if (player.direction.x != 0) {
			player.direction.x = 0;
		} else {
			player.direction.x = -1;
		}
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

	if (!keys[DIK_SPACE] && !preKeys[DIK_SPACE] ) {
		player.isJanp = false;
	}

	//プレイヤーの速度に加速度を足す
	player.velocity.x += player.acceleration.x;
	player.velocity.y -= player.acceleration.y;

	//プレイヤーの位置に速度を足す
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

void Player::Attack(Player_& player, Flash_& flash, const char* keys, const char* preKeys) {
	// 通常ライト
	if (keys[DIK_J] && !preKeys[DIK_J]) {
		if (!player.isFlash) {
			player.isFlash = true;
			player.isHighFlash = false;
		} else {
			player.isFlash = false;
		}
	}
	// 強化ライト
	if (keys[DIK_K] && !preKeys[DIK_K]) {
		if (!player.isHighFlash) {
			player.isHighFlash = true;
			player.isFlash = false;
		} else {
			player.isHighFlash = false;
		}
	}

	//フラッシュライトの位置を現在のプレイヤーの位置にする
	flash.pos = { player.pos.x + player.radius / 2.0f, player.pos.y + player.radius / 2.0f };

	//フラッシュライトの向きを最後にプレイヤーが向いていた方向にする
	if (player.direction.x != 0 || player.direction.y != 0) {
		flash.direction = player.direction;
	}
}

void Player::Draw(Player_& player) {
	Novice::DrawBox(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y),
		static_cast<int>(player.radius), static_cast<int>(player.radius), 0.0f, WHITE, kFillModeSolid);
}

void Player::DrawFlash(Player_& player, Flash_& flash) {
	// 通常ライト
	if (player.isFlash) {
		if (flash.direction.y > 0) {                                   //上
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.width), static_cast<int>(flash.pos.y + flash.range),
				static_cast<int>(flash.pos.x - flash.width), static_cast<int>(flash.pos.y + flash.range),
				0xe0dd8780, kFillModeSolid);
		} else if (flash.direction.y < 0) {                            //下
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.width), static_cast<int>(flash.pos.y - flash.range),
				static_cast<int>(flash.pos.x - flash.width), static_cast<int>(flash.pos.y - flash.range),
				0xe0dd8780, kFillModeSolid);
		}

		if (flash.direction.x > 0 && flash.direction.y == 0) {         //右
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.range), static_cast<int>(flash.pos.y + flash.width),
				static_cast<int>(flash.pos.x + flash.range), static_cast<int>(flash.pos.y - flash.width),
				0xe0dd8780, kFillModeSolid);
		} else if (flash.direction.x < 0 && flash.direction.y == 0) {  //左
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x - flash.range), static_cast<int>(flash.pos.y + flash.width),
				static_cast<int>(flash.pos.x - flash.range), static_cast<int>(flash.pos.y - flash.width),
				0xe0dd8780, kFillModeSolid);
		}
	}
	// 強化ライト
	if (player.isHighFlash) {
		if (flash.direction.y > 0) {                                   //上
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highWidth), static_cast<int>(flash.pos.y + flash.highRange),
				static_cast<int>(flash.pos.x - flash.highWidth), static_cast<int>(flash.pos.y + flash.highRange),
				0xe0dd87d0, kFillModeSolid);
		} else if (flash.direction.y < 0) {                            //下
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highWidth), static_cast<int>(flash.pos.y - flash.highRange),
				static_cast<int>(flash.pos.x - flash.highWidth), static_cast<int>(flash.pos.y - flash.highRange),
				0xe0dd87d0, kFillModeSolid);
		}

		if (flash.direction.x > 0 && flash.direction.y == 0) {         //右
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x + flash.highRange), static_cast<int>(flash.pos.y + flash.highWidth),
				static_cast<int>(flash.pos.x + flash.highRange), static_cast<int>(flash.pos.y - flash.highWidth),
				0xe0dd87d0, kFillModeSolid);
		} else if (flash.direction.x < 0 && flash.direction.y == 0) {  //左
			Novice::DrawTriangle(static_cast<int>(flash.pos.x), static_cast<int>(flash.pos.y),
				static_cast<int>(flash.pos.x - flash.highRange), static_cast<int>(flash.pos.y + flash.highWidth),
				static_cast<int>(flash.pos.x - flash.highRange), static_cast<int>(flash.pos.y - flash.highWidth),
				0xe0dd87d0, kFillModeSolid);
		}
	}
}
