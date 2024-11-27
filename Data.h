#pragma once
#include <Vector2.h>
#include "Novice.h"

const int MAX_BEAMS = 10;  // 同時に存在できる最大ビーム数
const int MAX_PROJECTILES = 3;
const int MAX_PROJECTILES2 = 4;
// 最大ビーム数
const int MAX_ALLRANGE_BEAMS = 30;
const int MAX_boll = 30; // 発射する弾の数

const int MAX_PARTICLES = 50; // パーティクルの最大数


// エリア全体
struct Whole {
	Vector2 pos = { 0.0f, 0.0f };
	Vector2 velocity = { 0.0f, 0.0f };
	bool isAttak = false;
	int attakTime = 360;
	int bounceCount = 0; // 反射回数を追加
};
// 発射された弾の配列

// プレイヤー
struct Player_ {
	Vector2 pos = { 32.0f,600.0f };
	Vector2 direction = { 1.0f, 0.0f };//プレイヤーの向き(1だったら右、-1だったら左向き)
	Vector2 velocity = { 0.0f, 0.0f };
	Vector2 acceleration = { 0.0f, 0.5f };
	float radius = 32;
	int hp = 300;    // ゲージの描画はUi.cpp内で調整してるので注意
	int energy = 600;// 同上
	const int kMaxEnergy = 600;
	int isFlash = false;
	int isHighFlash = false;
	int isJanp = false;
	int janpNum = 0;
	float speed = 2.0f;
	int isJanpSpeed = false;
	int isHit = false;
	int imageRight = Novice::LoadTexture("./Resources/images/player_01.png");
	int imageUp = Novice::LoadTexture("./Resources/images/player_02.png");
	int imageLeft = Novice::LoadTexture("./Resources/images/player_03.png");
	int imageDown = Novice::LoadTexture("./Resources/images/player_04.png");
	int isRight = true;
	int isLeft = false;
	int hitStopTime = 0;
	int flashTime = 0;
	int highFlashTime = 0;

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
	Vector2 size = { 60.0f, 800.0f };    // ビームのサイズ
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

// 全方位攻撃用ビーム構造体
struct AllRange {
	Vector2 startPos;   // ビームの開始位置
	Vector2 endPos;     // ビームの終了位置
	Vector2 currentPos; // ビームの現在位置
	Vector2 direction;  // ビームの進行方向
	float speed;        // ビームの移動速度
	int isActive;       // ビームがアクティブかどうか（1: 有効、0: 無効）
	int lifeTime;       // ビームの寿命（フレーム単位で管理）
};

// パーティクル構造体
struct Particle {
	Vector2 pos;       // パーティクルの位置
	Vector2 velocity;  // パーティクルの速度
	float lifetime;    // パーティクルの寿命
	float size;        // パーティクルのサイズ
	bool isActive;     // パーティクルが有効か
};

// ボス
struct Boss_ {
	Vector2 pos = { 1000.0f, 472.0f };    // ボスの初期位置
	Vector2 top = { 1000.0f + 32.0f, 472.0f + 32.0f };
	Vector2 bottom = { 1000.0f + 32.0f, 472.0f + 96.0f };
	Vector2 size = { 64.0f, 128.0f };
	Vector2 shakePos = { 0.0f,0.0f };
	Vector2 velocity = { 20.0f,20.0f };
	int hp = 0;                         // ゲージの描画Ui.cpp内で調整してるので注意
	int isHitTop = false;
	int isHitLeft = false;
	int isHitRight = false;
	int isHit = false;
	int attakNo = 0;
	int attakStandTime = 120;
	int isAttak = false;
	int secondHp = 200;
	int explosiveTime = 600;
	int isExplo = false;
	int isEase = false;
	float speed = 5.0f;
	int vanishTime = 0;//ボスを消す
	bool isFloating = false;          // 浮上状態か
	int attackCount = 0;              // 飛び道具の発射回数
	Beam_ beams[MAX_BEAMS];// ビーム攻撃用の配列
	int localTimer = 0;
	AllRange allRangeBeams[MAX_ALLRANGE_BEAMS];//オールレンジアタックの攻撃用配列

	int isFall = false;//落下攻撃
	int isFallAttak = false;
	int fallCount = 0;
	int fallSpeed = 10;
	int fallCoolTime = 0;

	int isOneExtraAttak = false;
	int isTwoExtraAttak = false;
	int isThreeExtraAttak = false;

	int hitStopTime = 0;

	int warpAttak = false;
	int image = Novice::LoadTexture("./Resources/images/boss_01.png");
	int imageLeft = Novice::LoadTexture("./Resources/images/boss_01.png");
	int imageRight = Novice::LoadTexture("./Resources/images/boss_02.png");
	int imageDown = Novice::LoadTexture("./Resources/images/boss_03.png");

	int imageBeam = Novice::LoadTexture("./Resources/images/beam.png");

	Whole bullets[MAX_boll];

	Particle particles[MAX_PARTICLES]; // パーティクル管理配列

	bool hasMovedToCenter = false;    // ボスが上中央に移動したか
	bool isReturning = false;         // ボスが降りている状態か

	// すべての弾が無効化されているか確認する関数
	bool AreAllBulletsInactive() {
		for (int i = 0; i < MAX_boll; i++) {
			if (bullets[i].isAttak) { // 修正：isAttack -> isAttak
				return false;
			}
		}

		return true;
	}

	float auraTimer = 0.0f; // オーラのフェードイン・アウト用タイマー

};
//ワープ攻撃
struct WarpAttak {
	Vector2 pos = { -100.0f,-100.0f };//bossの位置をここに固定する
	int attakTime = 0;//if(warp.attakTime>0)の場合プレイヤーの後ろに固定させる
	int isAttak = false;//trueだったら近接攻撃(shortDirectionAttak)

};

//落下攻撃の衝撃波
struct Shockwave {
	Vector2 pos = { -100.0f,-100.0f };       // 衝撃波の位置
	Vector2 size = { 32.0f,32.0f };      // 衝撃波のサイズ
	Vector2 direction; // 衝撃波の進行方向
	float speed = 10.0f;       // 衝撃波のスピード
	int isActive = false;      // 衝撃波が有効かどうか（1: 有効, 0: 無効）
	int isFalling = false;               // 球が落下中かどうか（1: 落下, 0: 非落下）
	int lifetime = 0;                    // 球の存在時間
	int imageWave = Novice::LoadTexture("./Resources/images/wave.png");
};

struct BossExprosive {
	Vector2 easePos = { 620.0f,100.0f };
	Vector2 safePos = { -100.0f,-100.0f };
	Vector2 attakSize = { 1280.0f,720.0f };
	Vector2 safeSize = { 128.0f,128.0f };
	int attakCoolTime = 120;
	int attakTime = 60;
	int isAttak = false;
	int isEase = false;
	int isDraw = false;

	int imageExplosive = Novice::LoadTexture("./Resources/images/explosiveAttak.png");
};

// 範囲攻撃
struct BossRengeAttak_ {
	Vector2 pos = { -32.0f, 0.0f };
	Vector2 size = { 480.0f, 2000.0f };
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

	int imageWepon = Novice::LoadTexture("./Resources/images/shortWepon.png");
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
	int isHit = false;

	int imageWepon = Novice::LoadTexture("./Resources/images/shortWepon.png");
};

// チャージ攻撃
struct Object {
	Vector2 pos = { 0.0f, 0.0f };
	Vector2 size = { 100,100 };
	Vector2 velocity = { 10.0f,10.0f };
	float floatHeight = 50.0f;
	bool isFloating = true;
	float throwSpeed = 15.0f;
	float rotation = 0.0f;
	float orbitRadius = 100.0f;
	int isAttak = false;
	int attakTime = 360;
	int attakStandTime = 30;
	int timer = 0;
};


struct Projectile {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 velocity = { 0.0f,0.0f };
	int radius = 20;
	bool isActive = false;
	int localTimer = 0;

};

struct  Shake {
	Vector2 pos = { 0.0f,0.0f };
	Vector2 bossPos = { 0.0f,0.0f };
	Vector2 bgPos = { 0.0f,0.0f };
	int time = 0;
};

// 地面
struct Line {
	Vector2 pos = { 0.0f, 600.0f };
	Vector2 endPos = { 1280.0f,600 };

	int title = Novice::LoadTexture("./Resources/images/title.png");
	int backGround = Novice::LoadTexture("./Resources/images/backGround.png");
	int guide = Novice::LoadTexture("./Resources/images/guideBackGround.png");
	int clear = Novice::LoadTexture("./Resources/images/clear.png");
	int pressToSpace = Novice::LoadTexture("./Resources/images/pressToSpace.png");
	int start = Novice::LoadTexture("./Resources/images/start.png");
	int exit = Novice::LoadTexture("./Resources/images/exit.png");
};

struct Sounds {
	int titlePlayHandle = -1;
	int fastBattlePlayHandle = -1;
	int secondBattlePlayHandle = -1;
	int clearPlayHandle = -1;

	int titleBgm = Novice::LoadAudio("./Resources/sounds/TitleBGM.mp3");
	int fastBattleBgm = Novice::LoadAudio("./Resources/sounds/FirstBossBGM.mp3");
	int secondBattleBgm = Novice::LoadAudio("./Resources/sounds/SecondBossBGM.mp3");
	int clearBgm = Novice::LoadAudio("./Resources/sounds/ClearBGM.mp3");
};

