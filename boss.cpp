#include "Novice.h"
#include "Data.h"
#include "boss.h"
#include <cmath> // 距離計算のために必要

// ボスの動き
void Boss::BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player) {
	if (boss.attakStandTime > 0) {
		boss.attakStandTime--;
	} else {
		boss.isAttak = true;
	}

	// ボスの攻撃が有効になったら
	if (boss.isAttak) {
		if (boss.hp > 100) {
			boss.attakNo = 2;  // 近距離攻撃
		}
	}

	// 範囲攻撃処理
	if (boss.attakNo == 1) {
		renge.isAttak = true;
		if (renge.isAttak) {
			if (renge.attakStandTime > 0) {
				renge.attakStandTime--;
			} else if (renge.attakTime > 0) {
				renge.attakTime--;
			} else {
				renge.isAttak = false;
				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120;
				renge.attakStandTime = 120;
			}
		}
	}



	// 近距離攻撃処理
	if (boss.attakNo == 2) {
		shortDist.isAttak = true;

		if (shortDist.isAttak) {
			// 攻撃中はボスを動かさない
			if (shortDist.attakTime == 30) {
				// プレイヤーとボスの距離を計算
				float distanceX = player.pos.x - boss.pos.x;
				float distanceY = player.pos.y - boss.pos.y;
				float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

				float stopDistance = 100.0f; // プレイヤーからこの距離で停止

				if (distance > stopDistance) {
					 if (shortDist.isEase) {
						 // ボスがプレイヤーに近づく（一定距離以上の場合）
					boss.pos.x += distanceX * shortDist.easeSpeed;
					boss.pos.y += distanceY * shortDist.easeSpeed;

					// 攻撃判定をリセットして攻撃をまだ開始しない
					shortDist.attakTime = 30;
					 }
				} else {
					// 一定距離まで近づき停止した後、攻撃判定を1度だけ設定
					if (player.pos.x < boss.pos.x) {
						// プレイヤーが左側にいる場合、左側に攻撃判定
						shortDist.pos.x = boss.pos.x - shortDist.size.x;
					} else {
						// プレイヤーが右側にいる場合、右側に攻撃判定
						shortDist.pos.x = boss.pos.x + boss.size.x;
					}
					shortDist.pos.y = boss.pos.y + boss.size.y / 2 - shortDist.size.y / 2 - 32.0f;

					shortDist.isEase = false;

				}
			}

			// 攻撃判定を維持しつつ、時間を減らす
			
			if (shortDist.attakTime > 0) {
				if (!shortDist.isEase) {
					shortDist.attakTime--;
				}
			} else {
				// 攻撃終了
				shortDist.isAttak = false;
				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120;  // 攻撃後のクールダウン
				shortDist.attakTime = 30;   // 次回攻撃の準備
				shortDist.isEase = true;    //次回のイージングの準備
			}
		}

	}
	// デバッグ用出力
	Novice::ScreenPrintf(32, 32, "attakNo::%d", boss.attakNo);
	Novice::ScreenPrintf(32, 64, "bossStandTime::%d", boss.attakStandTime);
	Novice::ScreenPrintf(32, 160, "ShortDist isAttak::%d", shortDist.isAttak);
	Novice::ScreenPrintf(32, 192, "ShortDist attakTime::%d", shortDist.attakTime);
	Novice::ScreenPrintf(32, 224, "ShortDist pos.x::%.1f", shortDist.pos.x);
	Novice::ScreenPrintf(32, 256, "ShortDist pos.y::%.1f", shortDist.pos.y);
}

// ボスを描画する
void Boss::BossDraw(Boss_ boss) {
	Novice::DrawBox(static_cast<int>(boss.pos.x), static_cast<int>(boss.pos.y),
		static_cast<int>(boss.size.x), static_cast<int>(boss.size.y), 0.0f, WHITE, kFillModeSolid);
}

// 範囲攻撃を描画する
void Boss::DrawBossRengeAttak(BossRengeAttak_ renge) {
	if (renge.attakStandTime <= 0) {
		if (renge.attakTime > 0) {
			Novice::DrawBox(static_cast<int>(renge.pos.x), static_cast<int>(renge.pos.y),
				static_cast<int>(renge.size.x), static_cast<int>(renge.size.y), 0.0f, RED, kFillModeSolid);
		}
	}
}

// 近距離攻撃の描画
void Boss::DrawShortDistansAttak(ShortDistansAttak_ shortDist) {
	if (shortDist.isAttak) {
		if (!shortDist.isEase) {
			Novice::DrawBox(static_cast<int>(shortDist.pos.x), static_cast<int>(shortDist.pos.y),
				static_cast<int>(shortDist.size.x), static_cast<int>(shortDist.size.y), 0.0f, BLUE, kFillModeSolid);
		}
	}
}
