#pragma once
#include <Vector2.h>

//同時に存在できる最大ビーム数
const int MAX_BEAMS = 10;

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
	float speed = 1.0f;
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

// ビーム攻撃用構造体
struct Beam_ {
	Vector2 pos = { 0.0f, 0.0f };         // ビームの位置
	Vector2 direction = { 0.0f, 1.0f };   // ビームの進行方向
	Vector2 size = { 100.0f, 800.0f };    // ビームのサイズ
	float speed = 5.0f;                   // ビームのスピード
	int attakTime = 60;                  // ビームの持続時間
	int attakStandTime = 120;
	int isAttak = false;                  // 攻撃中かどうか
};

// ボス
struct Boss_ {
	Vector2 pos = { 1000.0f, 472.0f };    // ボスの初期位置
	Vector2 size = { 64.0f, 128.0f };
	int hp = 200;
	int attakNo = 0;
	int attakStandTime = 120;
	int isAttak = false;

	Beam_ beams[MAX_BEAMS];// ビーム攻撃用の配列
};

// 範囲攻撃
struct BossRengeAttak_ {
	Vector2 pos = { -32.0f, 0.0f };
	Vector2 size = { 480.0f, 700.0f };
	int attakTime = 90;
	int attakStandTime = 120;
	int isAttak = false;
};

// 近距離攻撃
struct ShortDistansAttak_ {
	Vector2 pos = { 0.0f, 0.0f };        // 攻撃の位置
	Vector2 size = { 128.0f, 64.0f };    // 攻撃範囲のサイズ
	float easeSpeed = 0.05f;             // イージング速度
	float linearSpeed = 20.0f;            // 直線的な移動速度
	int attakTime = 10;                  // 攻撃判定の時間
	int isEase = true;                   // イージング状態かどうか
	int isAttak = false;                 // 攻撃中かどうか
	int attakCount = 0;                  // 攻撃回数カウント
	int expandDirection = 0;             // 攻撃範囲の拡大方向（-1: 左, 1: 右）
	float maxExpandSize = 200.0f;        // 攻撃範囲の最大サイズ
	float baseSizeX = 128.0f;            // 元のX方向のサイズ
	int isShortAttak = false;
};

// チャージ攻撃
struct Object {
    Vector2 pos;
    float floatHeight;
    bool isFloating;
    float throwSpeed;
    float rotation;
    float orbitRadius;
    int isAttak = false;
    int attakTime = 360;
    int attakStandTime = 30;
};

// 地面
struct Line {
	Vector2 pos = { 0.0f, 600.0f };
};

// timer の宣言（定義はソースファイルに）
extern int timer;
