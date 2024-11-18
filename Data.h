#pragma once
#include <Vector2.h>
#include <Novice.h>

// プレイヤー
struct Player_ {
	Vector2 pos = { 32.0f,600.0f };
	Vector2 direction = { 0.0f, 0.0f };
	Vector2 velocity = { 0.0f, 0.0f };
	Vector2 acceleration = { 0.0f, 0.5f };
	float radius = 32;
	int isFlash = false;
	int isJanp = false;
	int janpNum = 0;
	float speed = 2.0f;
	int isJanpSpeed = false;
};

//フラッシュライト
struct Flash_ {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 direction = { 0.0f, 0.0f };
	float range = 192;
	float width = 50;
	int damage = 1;
};

// ボス
struct Boss_ {
	Vector2 pos = { 1000.0f, 600.0f - 128.0f };
	Vector2 size = { 64.0f, 128.0f };
	int hp = 200;
	int attakNo = 0;
	int attakStandTime = 120;
	int isAttak = false;
};

// 範囲攻撃
struct BossRengeAttak_ {
	Vector2 pos = { -32.0f, 0.0f };
	Vector2 size = { 500.0f, 700.0f };
	int attakTime = 90;
	int attakStandTime = 60;
	int isAttak = false;
};

// 近距離攻撃
struct ShortDistansAttak_ {
	Vector2 pos = { 0.0f, 0.0f };  // 攻撃判定の位置
	Vector2 size = { 128.0f, 64.0f };  // 攻撃判定のサイズ
	int attakTime = 30;  // 攻撃の持続時間
	int isAttak = false;  // 攻撃フラグ
	int isEase = true;
	float easeSpeed = 0.05f;  // イージングのスピード
};

// 地面ライン
struct Line {
	Vector2 pos = { 0.0f, 600.0f };
};
