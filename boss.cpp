#include "Novice.h"
#include "Data.h"
#include "boss.h"

#include <cmath> // 距離計算のために必要
int timer = 0;  // timer をここで定義
// ボスのイージング移動処理
void Boss::BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed) {
	// イージングによる位置更新
	boss.pos.x += (targetPos.x - boss.pos.x) * easingSpeed;
	boss.pos.y += (targetPos.y - boss.pos.y) * easingSpeed;

	// 目標位置にほぼ到達したら、正確な位置にスナップ
	if (std::abs(targetPos.x - boss.pos.x) < 1.0f && std::abs(targetPos.y - boss.pos.y) < 1.0f) {
		boss.pos = targetPos;
	}
}


// ボスの動き
void Boss::BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player) {
	if (boss.attakStandTime > 0) {
		boss.attakStandTime--;
	} else {
		boss.isAttak = true;
	}

	//HPが50%以上ならattakNo1~3
	//%50%以下なら3~6(仮)
	//25%以下なら5~8(仮)
	if (boss.isAttak) {
		if (boss.hp > 100) {
			boss.attakNo = 5;
		}
	}

	// 範囲攻撃処理
	if (boss.attakNo == 1) {
		renge.isAttak = true;
		renge.pos.x = boss.pos.x - 200.0f;
		renge.pos.y = boss.pos.y - 572.0f;
		if (renge.isAttak) {
			if (renge.attakStandTime > 0) {
				renge.attakStandTime--;
				renge.attakTime = 90;
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
			if (shortDist.attakTime == 10) {
				// プレイヤーとボスの距離を計算
				float distanceX = player.pos.x - boss.pos.x;
				float distanceY = player.pos.y - boss.pos.y;
				float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

				float stopDistance = 150.0f; // プレイヤーからこの距離で停止

				if (distance > stopDistance) {
					if (shortDist.isEase) {
						// ボスがプレイヤーに近づく（一定距離以上の場合）
						boss.pos.x += distanceX * shortDist.easeSpeed;
						boss.pos.y += distanceY * shortDist.easeSpeed;

						// 攻撃判定をリセットして攻撃をまだ開始しない
						shortDist.attakTime = 10;
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
				shortDist.attakTime = 10;   // 次回攻撃の準備
				shortDist.isEase = true;    //次回のイージングの準備
			}
		}

	}

	// ビーム攻撃処理 (attakNo == 4)
	if (boss.attakNo == 4) {
		Vector2 targetPos = { 640.0f, 100.0f };  // 画面中央上部
		BossMoveToPosition(boss, targetPos, 0.05f);

		if (boss.pos.x == targetPos.x && boss.pos.y == targetPos.y) {
			BeamAttack(boss);  // ビーム攻撃開始
		}

		DrawBeams(boss);

	}

	// 連続近距離攻撃処理
	// 連続近距離攻撃処理
	if (boss.attakNo == 5) {
		shortDist.isAttak = true;

		if (shortDist.isAttak) {
			if (shortDist.attakCount == 0) {
				// 最初の攻撃: プレイヤーにイージングで近づく
				float distanceX = player.pos.x - boss.pos.x;
				float stopDistance = 100.0f; // プレイヤーからこの距離で停止

				if (std::abs(distanceX) > stopDistance) {
					// プレイヤーに近づく
					if (shortDist.isEase) {
						boss.pos.x += distanceX * shortDist.easeSpeed;
					}
					shortDist.attakTime = 10; // 攻撃判定をリセット
					shortDist.isShortAttak = false;
				} else {
					// 一定距離まで近づいたら攻撃
					if (shortDist.attakTime == 10) {
						if (player.pos.x < boss.pos.x) {
							shortDist.pos.x = boss.pos.x - shortDist.size.x;
							shortDist.expandDirection = -1; // 左方向
						} else {
							shortDist.pos.x = boss.pos.x + boss.size.x;
							shortDist.expandDirection = 1;  // 右方向
						}
						shortDist.pos.y = boss.pos.y + boss.size.y / 2 - shortDist.size.y / 2;
					}

					if (shortDist.attakTime > 0) {
						shortDist.isShortAttak = true;
						shortDist.attakTime--;
					} else {
						shortDist.attakCount++;
						shortDist.attakTime = 10; // 次回攻撃の準備
					}
				}

			} else if (shortDist.attakCount == 1) {
				// 2回目の攻撃: 横方向に攻撃範囲を拡大
				if (shortDist.attakTime == 10) {
					shortDist.size.x = shortDist.baseSizeX; // サイズをリセットして拡大開始
				}

				if (shortDist.size.x < shortDist.maxExpandSize) {
					shortDist.size.x += 20.0f; // 攻撃範囲を拡大
					if (shortDist.expandDirection == -1) {
						// 左方向に拡大
						shortDist.pos.x = boss.pos.x - shortDist.size.x;
					} else {
						// 右方向に拡大
						shortDist.pos.x = boss.pos.x + boss.size.x;
					}
				}

				if (shortDist.attakTime > 0) {
					shortDist.isShortAttak = true;
					shortDist.attakTime--;
				} else {
					// 攻撃終了
					shortDist.isShortAttak = false;
					shortDist.isAttak = false;
					shortDist.attakCount = 0;
					shortDist.size.x = shortDist.baseSizeX; // サイズをリセット
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120; // クールダウン
				}
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
	Novice::ScreenPrintf(32, 288, "ShortDist pos.x::%.1f", shortDist.pos.x);
	Novice::ScreenPrintf(32, 320, "ShortDist pos.y::%.1f", shortDist.pos.y);






	//チャージ攻撃
	// Objectの初期設定（Bossの周囲で回転）
	Object object = {
		{ boss.pos.x + 100.0f, boss.pos.y }, // 初期位置（Bossの右側）
		50.0f,                               // 浮遊高さ
		true,                                // 浮遊状態
		15.0f,                               // 初期投げ飛ばしスピード
		0.0f,                                // 初期回転角度
		100.0f                               // 円運動の半径
	};

	if (boss.attakNo == 3) {
		object.isAttak = true;
		if (object.isAttak) {
			// `object`が5秒後に投げ飛ばされる準備
			timer++;
			if (timer >= 300) { // 約5秒後
				object.isFloating = false;
			}

			// アニメーション処理
			if (object.isFloating) {
				// `Boss_`の周りを回転する
				object.rotation += 0.05f; // 回転速度を調整
				object.pos.x = boss.pos.x + object.orbitRadius * cos(object.rotation);
				object.pos.y = boss.pos.y + object.orbitRadius * sin(object.rotation);
			} else {
				// 左に投げ飛ばすアニメーション
				object.pos.x -= object.throwSpeed;
				object.throwSpeed += 1.0f;  // 徐々に加速
			}

			if (object.attakTime > 0) {
				object.attakTime--;
			} else {
				object.isAttak = false;
				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120;  // 攻撃後のクールダウン
				object.attakTime = 360;
			}
		}
	}

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
	if (shortDist.isShortAttak) {
		if (shortDist.attakTime>0) {
			Novice::DrawBox(static_cast<int>(shortDist.pos.x), static_cast<int>(shortDist.pos.y),
				static_cast<int>(shortDist.size.x), static_cast<int>(shortDist.size.y), 0.0f, BLUE, kFillModeSolid);
		}
	}
}

void Boss::BeamAttack(Boss_& boss) {
	for (int i = 0; i < MAX_BEAMS; i++) {
		if (!boss.beams[i].isAttak) {
			boss.beams[i].isAttak = true;
			boss.beams[i].pos = { boss.pos.x - (MAX_BEAMS / 2) * 125 + i * 130, boss.pos.y + boss.size.y };
			boss.beams[i].direction = { 0, 1 };
			boss.beams[i].speed = 5.0f;
			boss.beams[i].size = { 60.0f, 800.0f };
			boss.beams[i].attakStandTime = 100;
		}

		if (boss.beams[i].attakStandTime > 0) {
			boss.beams[i].attakStandTime--;
			boss.beams[i].attakTime = 60;
		} else if (boss.beams[i].attakStandTime <= 0) {
			if (boss.beams[i].attakTime > 0) {

				boss.beams[i].attakTime--;
			} else {
				boss.beams[i].isAttak = false;
				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120;
			}
		}

	}
}

void Boss::DrawBeams(Boss_& boss) {
	for (int i = 0; i < MAX_BEAMS; i++) {
		if (boss.beams[i].attakStandTime <= 0) {
			if (boss.beams[i].attakTime > 0) {
				Novice::DrawBox(static_cast<int>(boss.beams[i].pos.x), static_cast<int>(boss.beams[i].pos.y),
					static_cast<int>(boss.beams[i].size.x), static_cast<int>(boss.beams[i].size.y),
					0.0f, RED, kFillModeSolid);
			}
		} else if (boss.beams[i].attakStandTime > 0) {
			Novice::DrawBox(static_cast<int>(boss.beams[i].pos.x), static_cast<int>(boss.beams[i].pos.y),
				static_cast<int>(boss.beams[i].size.x), static_cast<int>(boss.beams[i].size.y),
				0.0f, 0xFFFFFF50, kFillModeSolid);
		}
	}
}

// チャージ攻撃の描画
void Boss::DrawBossChargeAttak(const Object& object) {
    Novice::DrawBox(static_cast<int>(object.pos.x), static_cast<int>(object.pos.y),
        100, 100, object.rotation, BLUE, kFillModeSolid);
}
