
#pragma once
#include <Vector2.h>

const int MAX_BEAMS = 10;  // 同時に存在できる最大ビーム数
const int MAX_PROJECTILES = 3;
// プレイヤー
struct Player_ {
    Vector2 pos = { 32.0f,600.0f };
    Vector2 direction = { 0.0f, 0.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    Vector2 acceleration = { 0.0f, 0.5f };
    float radius = 64;
	int hp = 300;    // ゲージの描画はUi.cpp内で調整してるので注意
	int energy = 600;// 同上
	const int kMaxEnergy = 600;
    int isFlash = false;
    int isHighFlash = false;
    int isJanp = false;
    int janpNum = 0;
    float speed = 2.0f;
    int isJanpSpeed = false;
	int isHit=false;
	int image = Novice::LoadTexture("./Resources/images/player_01.png");

};

//フラッシュライト
struct Flash_ {
    Vector2 pos = { 0.0f,0.0f };
    Vector2 direction = { 0.0f, 0.0f };
    float range = 192;
    float highRange = 288;
    float width = 50;
    float highWidth = 100;
    int damage = 1;
};

// ビーム攻撃用構造体
struct Beam_ {
	Vector2 pos = { -1000.0f, -1000.0f };         // ビームの位置
	Vector2 direction = { 0.0f, 1.0f };   // ビームの進行方向
	Vector2 size = { 100.0f, 800.0f };    // ビームのサイズ
	float speed = 5.0f;                   // ビームのスピード
	int attakTime = 60;                  // ビームの持続時間
	int attakStandTime = 120;
	int isAttak = false;                  // 攻撃中かどうか
};

struct Beam2 {
	Vector2 pos = { -1000.0f, -1000.0f };         // ビームの位置
	Vector2 direction = { 0.0f, 1.0f };   // ビームの進行方向
	Vector2 size = { 100.0f, 800.0f };    // ビームのサイズ
	float speed = 5.0f;                   // ビームのスピード
	int attakTime = 0;                  // ビームの持続時間
	int attakStandTime = 0;
	int isAttak = false;                  // 攻撃中かどうか
	int isEase = false;
};

// ボス
struct Boss_ {
	Vector2 pos = { 1000.0f, 472.0f };    // ボスの初期位置
	Vector2 size = { 64.0f, 128.0f };
	Vector2 shakePos = { 0.0f,0.0f };
	int hp = 200;                         // ゲージの描画Ui.cpp内で調整してるので注意
	int attakNo = 0;
	int attakStandTime = 120;
	int isAttak = false;
	int secondHp = 200;
	int isEase = false;
	float speed=5.0f;
	int vanishTime = 0;//ボスを消す
	bool isFloating = false;          // 浮上状態か
	int attackCount = 0;              // 飛び道具の発射回数
	Beam_ beams[MAX_BEAMS];// ビーム攻撃用の配列
	int localTimer = 0;
	int image = Novice::LoadTexture("./Resources/images/boss_01.png");

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
	int attakTime = 0;                  // 攻撃判定の時間
	int isEase = true;                   // イージング状態かどうか
	int isAttak = false;                 // 攻撃中かどうか
	int isShortAttak = false;
	int isHit = false;
};

//連続攻撃
struct ShortDubleDistansAttak_ {
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
	int isHit=false;
};



// チャージ攻撃
struct Object {
    Vector2 pos= { 0.0f, 0.0f };
	Vector2 size = { 100,100 };
    float floatHeight=50.0f;
	bool isFloating = true;
	float throwSpeed=15.0f;
    float rotation=0.0f;
    float orbitRadius=100.0f;
    int isAttak = false;
    int attakTime = 360;
    int attakStandTime = 30;
	int timer = 0;
};

struct Projectile {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 velocity = { 0.0f,0.0f };
	bool isActive = false;
	int localTimer = 0;

};



struct  Shake{
	Vector2 pos = { 0.0f,0.0f };
	Vector2 bossPos = { 0.0f,0.0f };
	Vector2 bgPos = { 0.0f,0.0f };
};



// 地面
struct Line {
	Vector2 pos = { 0.0f, 600.0f };
	Vector2 endPos = { 1280.0f,600 };

	int backGround = Novice::LoadTexture("./Resources/images/backGround.png");

};




