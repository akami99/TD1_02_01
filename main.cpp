
#include <Novice.h>
#include <math.h>
#include "Data.h"
#include "Player.h"
#include "boss.h"
#include "Ui.h"
#include <algorithm>


const char kWindowTitle[] = "1221_霊障退治";

enum Scene {

	TITLE,
	SELECT,
	GUIDE,
	FASTBOSS,
	SECONDBOSS,
	CLEAR,
	GAMEOVER,
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player_ player_;
	Player player;
	Line line;
	Flash_ flash_;
	Boss boss;
	Boss_ boss_;
	BossRengeAttak_ rengeAttak_;
	ShortDistansAttak_ shortDistAttak_;  // 近距離攻撃用
	Ui ui;
	Object object_;
	ShortDubleDistansAttak_ doubleShort;
	Shake shake;
	Beam2 beam2;
	Projectile projectiles[10];
	Boss::UpdateProjectiles(projectiles);

	int scene = TITLE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene) {

		case TITLE:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = FASTBOSS;
			}
			break;

		case FASTBOSS:

			///
			/// ↓更新処理ここから
			///

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			// ボスの更新処理（範囲攻撃と近距離攻撃を含む）
			boss.BossMove(boss_, rengeAttak_, shortDistAttak_, player_, object_, doubleShort, shake, beam2,projectiles);

			if (boss_.hp <= 0) {
				scene = SECONDBOSS;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			player.DrawBackGround(line, shake);

			Novice::DrawLine(0, 600, 1280, 600, WHITE);

			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);

			// プレイヤーの描画
			player.Draw(player_);

			//ボスのビーム攻撃
			boss.DrawBeams(boss_);

			//第二形態のボスのビーム攻撃
			boss.DrawBeam2(beam2);

			// ボスの範囲攻撃描画
			boss.DrawBossRengeAttak(rengeAttak_);

			// ボスの描画
			boss.BossDraw(boss_, shake);

			//オールレンジアタック
			boss.DrawAllRangeAttack(boss_);

			//近距離攻撃の描画
			boss.DrawShortDistansAttak(shortDistAttak_);

			//連続攻撃の描画
			boss.DrawDoubleShortDistansAttak(doubleShort);

			boss.DrawBossChargeAttak(object_);

			// UIの描画
			ui.DrawGauge(340, 30, boss_.hp, 0);
			ui.DrawGauge(100, 620, player_.hp, 1);
			ui.DrawGauge(100, 670, player_.energy, 2);

			boss.UpdateProjectiles(projectiles);

			//Novice::ScreenPrintf(0, 0, "%d",boss.time);
			///
			/// ↑描画処理ここまで
			///

			break;

		case SECONDBOSS:

			if (boss_.secondHp <= 0) {
				scene = CLEAR;
			}

			break;
		}

		

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


