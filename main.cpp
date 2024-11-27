
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
	BOSSUP,
	SECONDBOSSUP,
	FASTBOSS,
	SECONDBOSS,
	CLEAR,
	FINISHBRO,
	GAMEOVER,
};

enum {
	TITLE_BGM,
	FIRST_BOSS_BGM,
	SECOND_BOSS_BGM,
	CLEAR_BGM,
	GAMEOVER_BGM,
	TOTAL_BGM
};

// オーディオハンドルを保存する配列
int bgmHandles[TOTAL_BGM];

// BGMファイルをロードする初期化処理を追加
void LoadBgm() {
	bgmHandles[TITLE_BGM] = Novice::LoadAudio("./Resources/sounds/TitleBGM.mp3");
	bgmHandles[FIRST_BOSS_BGM] = Novice::LoadAudio("./Resources/sounds/FirstBossBGM.mp3");
	bgmHandles[SECOND_BOSS_BGM] = Novice::LoadAudio("./Resources/sounds/SecondBossBGM.mp3");
	bgmHandles[CLEAR_BGM] = Novice::LoadAudio("./Resources/sounds/ClearBGM.mp3");
	bgmHandles[GAMEOVER_BGM] = Novice::LoadAudio("./Resources/sounds/GameOverBGM.mp3");
}



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

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
	Sounds sounds;
	//	Particle Particle;
	Whole whole;

	float easingSpeed = 0.05f;
	Vector2 firstPos = { 600.0f,50.0f };

	int scene = TITLE;

	int isFinish = false;
	int sceneNo = 0;

	int gameOverGb = Novice::LoadTexture("./Resources/images/gameOver.png");

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// BGMをロード
		LoadBgm();

		switch (scene) {
		case TITLE:
			//クリアBGMを止める
			Novice::StopAudio(sounds.clearPlayHandle);
			Novice::StopAudio(sounds.gameoverPlayHandle);

			isFinish = false;

			if (keys[DIK_W] && !preKeys[DIK_W] || keys[DIK_UP] && !preKeys[DIK_UP]) {
				sceneNo--;
			}

			if (keys[DIK_S] && !preKeys[DIK_S] || keys[DIK_DOWN] && !preKeys[DIK_DOWN]) {
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
					scene = GUIDE;
				}
			}

			InitializeGame(player_, flash_, boss_, line);


			// TITLE BGMの再生
			if (!Novice::IsPlayingAudio(sounds.titlePlayHandle) || sounds.titlePlayHandle == -1) {
				sounds.titlePlayHandle = Novice::PlayAudio(bgmHandles[TITLE_BGM], true, 0.5f); // 修正：ハンドルを使用
			}

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

			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			if (player_.pos.x > 1280.0f - player_.radius * 2 - 10.0f) {
				scene = BOSSUP;
				InitializeGame(player_, flash_, boss_, line);
			}

			//クリアBGMを止める
			Novice::StopAudio(sounds.clearPlayHandle);

			Novice::StopAudio(sounds.gameoverPlayHandle);

			//====================
			//描画処理
			//====================

				//BGMが鳴っていなかったらTitleを流す
			if (!Novice::IsPlayingAudio(sounds.titlePlayHandle) || sounds.titlePlayHandle == -1) {
				sounds.titlePlayHandle = Novice::PlayAudio(sounds.titleBgm, true, 0.5f);
			}

			player.DrawBackGround(line.guide);
			player.DrawFlash(player_, flash_);
			player.Draw(player_, flash_);
			break;

		case BOSSUP:

			boss_.hp++;

			if (boss_.hp > 200) {
				scene = FASTBOSS;
				boss_.hp = 200;
			}

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);

			//======================
			//描画処理
			//======================

			player.DrawBackGround(line, shake);

			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);

			// プレイヤーの描画
			player.Draw(player_, flash_);

			boss.BossDraw(boss_, shake);
			// UIの描画
			ui.DrawGauge(340, 30, boss_.hp, 0);

			break;

		case FASTBOSS:

			///
			/// ↓更新処理ここから
			///

			if (boss_.hitStopTime > 0) {
				boss_.hitStopTime--;
			}

			if (boss_.hitStopTime > 0) boss_.hitStopTime--;
			if (player_.hitStopTime > 0) player_.hitStopTime--;

			if (boss_.hitStopTime <= 0) {
				if (player_.hitStopTime <= 0) {
					// プレイヤーの更新処理
					player.Move(player_, line, keys, preKeys);


					// ボスの更新処理（範囲攻撃と近距離攻撃を含む）
					boss.BossMove(boss_, rengeAttak_, shortDistAttak_, player_, object_, doubleShort, shake, projectiles);
					player.Attack(player_, boss_, flash_, keys, preKeys);
				}
			}
			if (boss_.hp <= 0) {
				scene = SECONDBOSSUP;
				boss_.attakNo = 0;
				boss_.attakStandTime = 120;
				boss_.isAttak = false;
			}
			Novice::StopAudio(sounds.titlePlayHandle);

			if (!Novice::IsPlayingAudio(sounds.fastBattlePlayHandle) || sounds.fastBattlePlayHandle == -1) {
				sounds.fastBattlePlayHandle = Novice::PlayAudio(bgmHandles[FIRST_BOSS_BGM], true, 0.5f); // 修正：ハンドルを使用
			}

			if (player_.hp <= 0) {
				scene = GAMEOVER;
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
			player.Draw(player_,flash_);

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

			//衝撃波の描画
			boss.DrawShockwaves(shockwaves, 10);

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

		case SECONDBOSSUP:

			boss_.pos.x += (firstPos.x - boss_.pos.x) * easingSpeed;
			boss_.pos.y += (firstPos.y - boss_.pos.y) * easingSpeed;

			if (std::abs(firstPos.x - boss_.pos.x) < 0.5f && std::abs(firstPos.y - boss_.pos.y) < 0.5f) {
				boss_.pos = firstPos;

				boss_.hp++;
				if (boss_.hp > 200) {
					scene = SECONDBOSS;
					boss_.hp = 200;
				}


			}
			Novice::StopAudio(sounds.fastBattlePlayHandle);

			if (!Novice::IsPlayingAudio(sounds.secondBattlePlayHandle) || sounds.secondBattlePlayHandle == -1) {
				sounds.secondBattlePlayHandle = Novice::PlayAudio(bgmHandles[SECOND_BOSS_BGM], true, 0.5f); // 修正：ハンドルを使用
			}
			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);

			if (player_.hp <= 0) {
				scene = GAMEOVER;
			}

			//======================
			//描画処理
			//======================

			player.DrawBackGround(line, shake);

			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);

			// プレイヤーの描画
			player.Draw(player_, flash_);

			boss.BossDraw(boss_, shake);
			// UIの描画
			ui.DrawGauge(340, 30, boss_.hp, 0);
			ui.DrawGauge(100, 620, player_.hp, 1);
			ui.DrawGauge(100, 670, player_.energy, 2);
			break;

		case SECONDBOSS:
			Novice::StopAudio(sounds.fastBattlePlayHandle);

			if (!Novice::IsPlayingAudio(sounds.secondBattlePlayHandle) || sounds.secondBattlePlayHandle == -1) {
				sounds.secondBattlePlayHandle = Novice::PlayAudio(bgmHandles[SECOND_BOSS_BGM], true, 0.5f); // 修正：ハンドルを使用
			}

			if (boss_.hitStopTime > 0) boss_.hitStopTime--;
			if (player_.hitStopTime > 0) player_.hitStopTime--;

			// プレイヤーの更新処理
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);

			boss.SecondBossMove(boss_, shortDistAttak_, player_, shake, beam2, shockwaves, warp, explosive, projectiles);


			if (player_.hp <= 0) {
				scene = GAMEOVER;
			}

			if (boss_.hp <= 0) {
				scene = FINISHBRO;
			}

			if (boss_.secondHp <= 0) scene = CLEAR;

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

			// UIの描画
			ui.DrawGauge(340, 30, boss_.hp, 0);
			ui.DrawGauge(100, 620, player_.hp, 1);
			ui.DrawGauge(100, 670, player_.energy, 2);

			break;

		case FINISHBRO:

			//================
			// 更新処理
			//================

			// ボスの終了処理（ライトが当たると反射しながら爆発）
			boss.BossFinishBro(boss_, player_, shake);

			// プレイヤーの移動処理（ライト操作や移動）
			player.Move(player_, line, keys, preKeys);
			player.Attack(player_, boss_, flash_, keys, preKeys);
			//======================
			// 描画処理
			//======================

			// 背景描画（揺れの演出込み）
			player.DrawBackGround(line, shake);

			// ボスの描画
			//boss.DrawParticles(boss_.particles, 50);  // ボスのパーティクル
			boss.BossDraw(boss_, shake);              // ボス本体の描画

			boss.DrawBossExplo(boss_);

			// フラッシュライトの描画
			player.DrawFlash(player_, flash_);
			// プレイヤーの描画
			player.Draw(player_, flash_);
			// エフェクトが発生中の場合



			if (boss_.explosiveTime <= 0) {
				scene = CLEAR;
			}

			break;
		case GAMEOVER:
			Novice::StopAudio(sounds.fastBattlePlayHandle);
			Novice::StopAudio(sounds.secondBattlePlayHandle);

			isFinish = false;
			if (!Novice::IsPlayingAudio(sounds.gameoverPlayHandle) || sounds.gameoverPlayHandle == -1) {
				sounds.gameoverPlayHandle = Novice::PlayAudio(bgmHandles[GAMEOVER_BGM], true, 0.5f);
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			//=====================
			//描画処理
			//=====================

			Novice::DrawSprite(0, 0, gameOverGb, 1, 1, 0.0f, WHITE);

			ui.DrawFont(410, 600, line.pressToSpace);


			break;



		case CLEAR:
			Novice::StopAudio(sounds.secondBattlePlayHandle);

			if (!Novice::IsPlayingAudio(sounds.clearPlayHandle) || sounds.clearPlayHandle == -1) {
				sounds.clearPlayHandle = Novice::PlayAudio(bgmHandles[CLEAR_BGM], true, 0.5f);
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			player.DrawBackGround(line.clear);
			ui.DrawFont(410, 600, line.pressToSpace);
			break;
		}


		Novice::EndFrame();

		if (isFinish) {
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				break;
			}
		}

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
