
#include <Novice.h>
#include <math.h>
#include "Data.h"
#include "Player.h"
#include "boss.h"
#include "Ui.h"
#include <algorithm>
#include "GameInitializer.h"

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

	Shockwave shockwaves[10]; // 衝撃波の配列

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
	BossExprosive explosive;
	WarpAttak warp;


	Whole whole;


	int scene = TITLE;

	int sceneNo = 0;
	int isFinish = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene) {

		case TITLE:
			isFinish = false;

			if (keys[DIK_W] && !preKeys[DIK_W] || keys[DIK_UP] && preKeys[DIK_UP]) {
				sceneNo--;
			}

			if (keys[DIK_S] && !preKeys[DIK_S] || keys[DIK_DOWN] && preKeys[DIK_DOWN]) {
				sceneNo++;
			}

			if (sceneNo < 0) {
				sceneNo = 1;
			}

			if (sceneNo > 1) {
				sceneNo = 0;
			}

			if (sceneNo == 1) {
				isFinish = true;
			}

			if (sceneNo == 0) {
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
					player.Initialize(player_, flash_);
					scene = GUIDE;
				}
			}


			//初期化処理
			InitializeGame(player_, flash_, boss_, line);

			player.DrawBackGround(line.title);

			if (sceneNo == 0) {
				ui.DrawFont(555, 325, line.start);
				ui.DrawLightFont(580, 455, line.exit);
			} else if (sceneNo == 1) {
				ui.DrawLightFont(555, 325, line.start);
				ui.DrawFont(580, 455, line.exit);
			}

			ui.DrawFont(410, 600, line.pressToSpace);
			break;

		case GUIDE:

			//====================
			//更新処理
			//====================

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			if (player_.pos.x > 1280.0f - player_.radius * 2-10.0f) {
				scene = FASTBOSS;
				InitializeGame(player_, flash_, boss_, line);
			}

			//====================
			//描画処理
			//====================

			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);
			// プレイヤーの描画
			player.Draw(player_, flash_);


			break;


		case FASTBOSS:

			///
			/// ↓更新処理ここから
			///

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			// ボスの更新処理（範囲攻撃と近距離攻撃を含む）
			boss.BossMove(boss_, rengeAttak_, shortDistAttak_, player_, object_, doubleShort, shake, projectiles);

			if (boss_.hp <= 0) {
				scene = SECONDBOSS;
				boss_.attakNo = 0;
				boss_.attakStandTime = 120;
				boss_.isAttak = false;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			player.DrawBackGround(line, shake);

			Novice::DrawLine(0, 600, 1280, 600, WHITE);

			// ボスの描画
			boss.DrawParticles(boss_.particles, 50);
			//boss.DrawAura(boss_);
			boss.BossDraw(boss_, shake);
			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);

			// プレイヤーの描画
			player.Draw(player_, flash_);

			//ボスのビーム攻撃
			boss.DrawBeams(boss_);

			// ボスの範囲攻撃描画
			boss.DrawBossRengeAttak(rengeAttak_);

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


			// 弾の描画
			boss.DrawWhole();




			//Novice::ScreenPrintf(0, 0, "%d",boss.time);
			///
			/// ↑描画処理ここまで
			///

			break;

		case SECONDBOSS:

			//================================
			//更新処理
			//================================

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			boss.SecondBossMove(boss_, shortDistAttak_, player_, shake, beam2, shockwaves, warp, explosive,projectiles);

			if (boss_.secondHp <= 0) {
				scene = CLEAR;
			}

			//========================
			//描画処理
			//========================

			player.DrawBackGround(line, shake);

			Novice::DrawLine(0, 600, 1280, 600, WHITE);


			player.DrawFlash(player_, flash_);

			// プレイヤーの描画
			player.Draw(player_, flash_);

			// ボスの描画
			boss.DrawParticles(boss_.particles, 50);
			//boss.DrawAura(boss_);
			boss.BossDraw(boss_, shake);
		
			//第二形態のボスのビーム攻撃
			boss.DrawBeam2(beam2);

			//オールレンジアタック
			boss.DrawAllRangeAttack(boss_);


			//近距離攻撃の描画
			boss.DrawShortDistansAttak(shortDistAttak_);

			//衝撃波の描画
			boss.DrawShockwaves(shockwaves, 10);

			boss.DrawWarpAttak(warp, shortDistAttak_);

			boss.DrawExplosive(explosive);


			break;
		case CLEAR:

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			player.DrawBackGround(line.clear);

			ui.DrawFont(410, 600, line.pressToSpace);
			break;
		}



		// フレームの終了
		Novice::EndFrame();

		if (isFinish) {
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				break;
			}
		}

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

