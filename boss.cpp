#include "Novice.h"
#include "Data.h"
#include "boss.h"
#include <cmath> // 距離計算のために必要
#include <algorithm>


Whole bullets[MAX_boll];
// ボスのイージング移動処理
void Boss::BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed) {
	boss.pos.x += (targetPos.x - boss.pos.x) * easingSpeed;
	boss.pos.y += (targetPos.y - boss.pos.y) * easingSpeed;

	if (std::abs(targetPos.x - boss.pos.x) < 0.5f && std::abs(targetPos.y - boss.pos.y) < 0.5f) {
		boss.pos = targetPos;
	}
}


// ボスの動き
void Boss::BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player, Object& object, ShortDubleDistansAttak_& doubleShort, Shake& shake, Beam2& beam2, Projectile* projectiles, Shockwave* shockwaves, WarpAttak& warp) {

	if (boss.attakStandTime > 0) {
		boss.attakStandTime--;
	} else {
		boss.isAttak = true;
	}

	if (boss.vanishTime > 0) {
		boss.vanishTime--;
	}

	if (boss.vanishTime == 0) {
		//HPが50%以上ならattakNo1~3
		//%50%以下なら3~6(仮)
		//25%以下なら5~8(仮)
		if (!boss.isAttak) {
			if (boss.attakNo == 0) {
				if (boss.attakStandTime <= 0) {
					//boss.attakNo = rand() % 5 + 1;
					boss.attakNo = 40;
					/*if (boss.hp > 100) {
						boss.attakNo = 5;
					}*/
					boss.isEase = false;
				}
			}
		}
	}

	// 範囲攻撃処理
	if (boss.attakNo == 1) {
		BossRengeAttak(boss, renge, shake);
		PlayerRengeHitBox(player, renge);
	}

	// 近距離攻撃処理
	if (boss.attakNo == 2) {

		ShortDistansAttak(boss, shortDist, player);

		PlayerShortAttakHitBox(player, shortDist);

	}

	// ビーム攻撃処理 
	if (boss.attakNo == 4) {

		Vector2 targetPos = { 640.0f, 100.0f };  // 画面中央上部
		if (!boss.isEase) {
			BossMoveToPosition(boss, targetPos, 0.05f);
		}

		if (boss.pos.x == targetPos.x && boss.pos.y == targetPos.y) {
			boss.isEase = true;
			BeamAttack(boss, shake);// ビーム攻撃開始
		}

		PlayerBeamsHitBox(player, boss);
		Novice::ScreenPrintf(200, 500, "%d", player.isHit);
	}

	// 連続近距離攻撃処理
	if (boss.attakNo == 5) {

		DoubleShortDistansAttak(boss, doubleShort, player);

		PlayerShortDobleAttakHitBox(player, doubleShort);
		Novice::ScreenPrintf(200, 500, "%d", player.isHit);
	}

	// デバッグ用出力
	Novice::ScreenPrintf(32, 32, "attakNo::%d", boss.attakNo);
	Novice::ScreenPrintf(32, 64, "bossStandTime::%d", boss.attakStandTime);

	if (boss.attakNo == 3) {


		object.isAttak = true;

		object.timer++;

		if (object.isAttak) {
			// タイマーの進行
			object.timer++;

			// 浮遊状態と飛ばす動きの切り替え
			if (object.timer >= 300) { // 5秒後
				object.isFloating = false;
			}

			// 浮遊状態の処理
			if (object.isFloating) {
				// ボスの周囲を回転
				object.rotation += 0.05f; // 回転速度
				object.pos.x = boss.pos.x + object.orbitRadius * cos(object.rotation);
				object.pos.y = boss.pos.y + object.orbitRadius * sin(object.rotation);
			} else {
				// 発射タイミングで一度だけ初速度を設定
				if (object.timer == 300) { // 飛ばすタイミング
					Vector2 direction = { player.pos.x - object.pos.x, player.pos.y - object.pos.y };
					float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

					// ベクトルを正規化して速度を計算
					if (length != 0) {
						direction.x /= length;
						direction.y /= length;
					}

					// オブジェクトに速度を設定
					object.velocity = { direction.x * object.throwSpeed, direction.y * object.throwSpeed };
				}

				// 設定された速度で移動
				object.pos.x += object.velocity.x;
				object.pos.y += object.velocity.y;

				// プレイヤーに接近したら攻撃終了
				float stopThreshold = 50.0f; // 近接判定のしきい値
				float distanceToPlayer = std::sqrt(
					(player.pos.x - object.pos.x) * (player.pos.x - object.pos.x) +
					(player.pos.y - object.pos.y) * (player.pos.y - object.pos.y));

				if (distanceToPlayer < stopThreshold) {
					// 攻撃終了処理
					object.isAttak = false;
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120; // クールダウン時間
					object.timer = 0;
					object.rotation = 0.0f;
					object.throwSpeed = 15.0f; // 初期スピードに戻す
					object.pos.x = boss.pos.x + object.orbitRadius;
					object.pos.y = boss.pos.y + object.orbitRadius;
					object.isFloating = true;
				}

				// 画面外判定
				float screenWidth = 1330.0f;  // 画面幅（例）
				float screenHeight = 770.0f; // 画面高さ（例）
				bool isOutOfScreen = object.pos.x < -50 || object.pos.x > screenWidth ||
					object.pos.y < -50 || object.pos.y > screenHeight;
				if (isOutOfScreen) {
					// 攻撃終了処理
					object.isAttak = false;
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120; // クールダウン時間
					object.timer = 0;
					object.rotation = 0.0f;
					object.throwSpeed = 15.0f; // 初期スピードに戻す
					object.pos.x = boss.pos.x + object.orbitRadius;
					object.pos.y = boss.pos.y + object.orbitRadius;
					object.isFloating = true;
				}
			}
		}


	}

	if (boss.attakNo == 10) {
		Beam2Attak(boss, beam2, shake);

	}

	if (boss.attakNo == 11) {
		boss.localTimer++;
		if (!boss.isFloating) {
			if (boss.localTimer >= 5) {
				boss.isFloating = true;
				boss.localTimer = 0;
			}
		} else {
			if (boss.pos.y > 400.0f && boss.attackCount < MAX_PROJECTILES) {
				boss.pos.y -= 2.0f;
			} else if (boss.attackCount < MAX_PROJECTILES) {
				if (boss.localTimer >= 20) {
					projectiles[boss.attackCount].pos = boss.pos;
					Vector2 direction = { player.pos.x - boss.pos.x, player.pos.y - boss.pos.y };
					float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
					direction.x /= length;
					direction.y /= length;

					projectiles[boss.attackCount].velocity = { direction.x * 8.0f, direction.y * 8.0f };
					projectiles[boss.attackCount].isActive = true;
					boss.attackCount++;
					boss.localTimer = 0;
				}
			} else {
				if (boss.pos.y < 598.0f) {
					boss.pos.y += 2.0f;
				} else {
					boss.isFloating = false;
					boss.attackCount = 0;
				}
			}
		}
	}

	//エリア全体
	if (boss.attakNo == 12) {

		// ボスの移動処理（上中央に移動）
		if (!boss.hasMovedToCenter) {
			if (boss.pos.y > 240.0f) {
				boss.pos.y -= boss.speed;
			} else {
				boss.hasMovedToCenter = true;
				boss.isFloating = true;
			}
		}

		// ボスの攻撃処理
		if (boss.hasMovedToCenter && boss.attackCount == 0) {
			for (int i = 0; i < MAX_boll; i++) {
				boss.bullets[i].isAttak = true;
				boss.bullets[i].bounceCount = 0;
				float angle = (rand() % 360) * (3.14159f / 180.0f);
				boss.bullets[i].velocity.x = cos(angle) * 5.0f;
				boss.bullets[i].velocity.y = sin(angle) * 5.0f;
				boss.bullets[i].pos = boss.pos;
			}
			boss.attackCount = 1;
		}

		// 弾の更新処理
		for (int i = 0; i < MAX_boll; i++) {
			if (boss.bullets[i].isAttak) {
				boss.bullets[i].pos.x += boss.bullets[i].velocity.x;
				boss.bullets[i].pos.y += boss.bullets[i].velocity.y;

				if (boss.bullets[i].pos.x < 0 || boss.bullets[i].pos.x > 1280) {
					boss.bullets[i].velocity.x = -boss.bullets[i].velocity.x;
					boss.bullets[i].bounceCount++;
				}

				if (boss.bullets[i].pos.y < 0 || boss.bullets[i].pos.y > 720) {
					boss.bullets[i].velocity.y = -boss.bullets[i].velocity.y;
					boss.bullets[i].bounceCount++;
				}

				if (boss.bullets[i].bounceCount >= 2) {
					boss.bullets[i].isAttak = false;
				}


			}
		}

		// ボスが降りる条件
		if (boss.isFloating && boss.attackCount > 0 && boss.AreAllBulletsInactive()) {
			boss.isReturning = true;
			boss.isFloating = false;
		}
		if (boss.isReturning) {
			if (boss.pos.y < 600.0f) {
				boss.pos.y += boss.speed;
			} else {
				boss.isReturning = false;
				boss.hasMovedToCenter = false;
				boss.attackCount = 0;
			}
		}
	}

		
	if (boss.attakNo == 20) { // 攻撃番号20を全方位攻撃に設定
		static bool initialized = false;

		// 初期化処理
		if (!initialized) {
			InitializeAllRangeAttack(boss); // 初期化関数でビームをセットアップ
			initialized = true;
		}

		// 更新処理
		UpdateAllRangeAttack(boss);

		// すべてのビームが終了したら攻撃終了
		bool allFinished = true;
		for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
			if (boss.allRangeBeams[i].isActive) {
				allFinished = false;
				break;
			}
		}

		// 全てのビームが終了していたら、攻撃をリセット
		if (allFinished) {
			initialized = false;  // 初期化フラグをリセット
			boss.attakNo = 0;     // 次の攻撃に移行
			boss.attakStandTime = 120; // クールダウン時間
			boss.isAttak = false;
		}
	}
	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
		if (boss.allRangeBeams[i].lifeTime <= 10) {
			AllRengeAttakHitBox(boss, player);
		}
	}

	//落下攻撃
	if (boss.attakNo == 30) {
		BossFallAttak(boss, player, shake, shockwaves, 10);

		UpdateShockwaves(shockwaves, 10);

	}

	if (boss.attakNo == 40) {
		BossWarpAttak(boss, player, warp, shortDist, shake);
	}



	Novice::ScreenPrintf(200, 500, "%d", player.isHit);

}



// ボスを描画する
void Boss::BossDraw(Boss_ boss, Shake& shake) {
	/*Novice::DrawBox(static_cast<int>(boss.pos.x), static_cast<int>(boss.pos.y),
		static_cast<int>(boss.size.x), static_cast<int>(boss.size.y), 0.0f, WHITE, kFillModeSolid);*/
	if (boss.vanishTime == 0) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x + shake.bossPos.x), static_cast<int>(boss.pos.y + shake.pos.y + shake.bossPos.y),
			boss.image, 1, 1, 0.0f, WHITE);
	} else if (boss.vanishTime < 10) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x + shake.bossPos.x), static_cast<int>(boss.pos.y + shake.pos.y + shake.bossPos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF50);
	} else if (20 > boss.vanishTime && boss.vanishTime > 10) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x + shake.bossPos.x), static_cast<int>(boss.pos.y + shake.pos.y + shake.bossPos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF40);
	} else if (30 > boss.vanishTime && boss.vanishTime > 20) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x + shake.bossPos.x), static_cast<int>(boss.pos.y + shake.pos.y + shake.bossPos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF30);
	}
}

///=============================================
///範囲攻撃の関数
///=============================================

//範囲攻撃の処理
void Boss::BossRengeAttak(Boss_& boss, BossRengeAttak_& renge, Shake& shake) {
	renge.isAttak = true;
	renge.pos.x = boss.pos.x - 200.0f;
	renge.pos.y = boss.pos.y - 572.0f;
	if (renge.isAttak) {
		if (renge.attakStandTime > 0) {
			renge.attakStandTime--;
			shake.pos.x = static_cast<float>(rand() % 20 - 10);
			shake.pos.y = static_cast<float>(rand() % 20 - 10);
			renge.attakTime = 90;
		} else if (renge.attakTime > 0) {
			shake.pos.x = static_cast<float>(rand() % 40 - 20);
			shake.pos.y = static_cast<float>(rand() % 40 - 20);
			renge.attakTime--;
		} else {
			renge.isAttak = false;
			boss.isAttak = false;
			boss.attakNo = 0;
			boss.attakStandTime = 120;
			renge.attakStandTime = 120;
			shake.pos = { 0.0f,0.0f };
		}
	}
}

// 範囲攻撃を描画する
void Boss::DrawBossRengeAttak(BossRengeAttak_ renge) {
	if (renge.attakStandTime <= 0) {
		if (renge.attakTime > 0) {
			if (renge.attakTime % 2 == 0) {
				Novice::DrawBox(static_cast<int>(renge.pos.x), static_cast<int>(renge.pos.y),
					static_cast<int>(renge.size.x), static_cast<int>(renge.size.y), 0.0f, RED, kFillModeSolid);
			}
		}
	}
}

//==========================
//近距離攻撃
//==========================
void Boss::ShortDistansAttak(Boss_& boss, ShortDistansAttak_& shortDist, Player_ player) {
	shortDist.isAttak = true;

	if (shortDist.isAttak) {
		// 攻撃中はボスを動かさない
	//	if (shortDist.attakTime == 10) {
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
				shortDist.attakTime = 20;
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
		//	}

			//攻撃判定を維持しつつ、時間を減らす
		if (shortDist.attakTime > 0) {
			if (!shortDist.isEase) {
				shortDist.attakTime--;
				//近距離攻撃の描画

			}
		} else {
			// 攻撃終了
			shortDist.isAttak = false;
			boss.isAttak = false;
			boss.attakNo = 0;
			boss.attakStandTime = 120;  // 攻撃後のクールダウン
			//shortDist.attakTime = 10;   // 次回攻撃の準備
			shortDist.isEase = true;    //次回のイージングの準備
			shortDist.isHit = false;
		}
	}
}


// 近距離攻撃の描画
void Boss::DrawShortDistansAttak(ShortDistansAttak_ shortDist) {
	if (!shortDist.isEase) {
		if (shortDist.attakTime > 0) {
			Novice::DrawBox(static_cast<int>(shortDist.pos.x), static_cast<int>(shortDist.pos.y),
				static_cast<int>(shortDist.size.x), static_cast<int>(shortDist.size.y), 0.0f, BLUE, kFillModeSolid);
		}
	}
}

//連続攻撃の描画
void Boss::DrawDoubleShortDistansAttak(ShortDubleDistansAttak_ dobleShort) {
	if (dobleShort.isShortAttak) {
		if (dobleShort.attakTime > 0) {
			Novice::DrawBox(static_cast<int>(dobleShort.pos.x), static_cast<int>(dobleShort.pos.y),
				static_cast<int>(dobleShort.size.x), static_cast<int>(dobleShort.size.y), 0.0f, BLUE, kFillModeSolid);
		}
	}
}

//=============================
//ビーム攻撃
//=============================

void Boss::BeamAttack(Boss_& boss, Shake& shake) {
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
			shake.pos.x = static_cast<float>(rand() % 20 - 10);
			shake.pos.y = static_cast<float>(rand() % 20 - 10);
			boss.beams[i].attakStandTime--;
			boss.beams[i].attakTime = 60;
		} else if (boss.beams[i].attakStandTime <= 0) {
			if (boss.beams[i].attakTime > 0) {
				shake.pos.x = static_cast<float>(rand() % 30 - 15);
				shake.pos.y = static_cast<float>(rand() % 30 - 15);
				boss.beams[i].attakTime--;
			} else {

				boss.beams[i].isAttak = false;
			}
		}
	}


	// すべてのビームが終了しているか確認
	bool allBeamsFinished = true;
	for (int i = 0; i < MAX_BEAMS; i++) {
		if (boss.beams[i].isAttak) {
			allBeamsFinished = false;
			break;
		}
	}

	if (allBeamsFinished) {
		// ボスを初期位置に戻すリセット処理
		static Vector2 originalPosition = { 1000.0f, 472.0f }; // ボスの初期位置を設定
		boss.pos = originalPosition;
		shake.pos.x = 0.0f;
		shake.pos.y = 0.0f;
		boss.vanishTime = 60;
		boss.isAttak = false;
		boss.attakNo = 0;
		boss.attakStandTime = 120; // クールダウン時間をリセット
	}


}

void Boss::DrawBeams(Boss_& boss) {
	for (int i = 0; i < MAX_BEAMS; i++) {
		if (boss.beams[i].attakStandTime <= 0) {
			if (boss.beams[i].attakTime > 0) {
				if (boss.beams[i].attakTime % 2 == 0) {
					Novice::DrawBox(static_cast<int>(boss.beams[i].pos.x), static_cast<int>(boss.beams[i].pos.y),
						static_cast<int>(boss.beams[i].size.x), static_cast<int>(boss.beams[i].size.y),
						0.0f, RED, kFillModeSolid);
				}
			}
		} else if (boss.beams[i].attakStandTime > 0) {
			Novice::DrawBox(static_cast<int>(boss.beams[i].pos.x), static_cast<int>(boss.beams[i].pos.y),
				static_cast<int>(boss.beams[i].size.x), static_cast<int>(boss.beams[i].size.y),
				0.0f, 0xFFFFFF50, kFillModeSolid);
		}
	}
}

void Boss::Beam2Attak(Boss_& boss, Beam2& beam2, Shake& shake) {
	if (!beam2.isEase) {
		Vector2 targetPos = { 640.0f, 100.0f };  // 画面中央上部

		boss.pos.x += (targetPos.x - boss.pos.x) * 0.05f;
		boss.pos.y += (targetPos.y - boss.pos.y) * 0.05f;
		boss.vanishTime = 2;
		if (std::abs(targetPos.x - boss.pos.x) < 0.5f && std::abs(targetPos.y - boss.pos.y) < 0.5f) {
			boss.pos = targetPos;
			beam2.pos = boss.pos;
			beam2.isEase = true;
		}

		beam2.attakStandTime = 60;
		beam2.attakTime = 300;

	} else if (beam2.isEase) {

		if (beam2.attakStandTime > 0) {
			beam2.attakStandTime--;
			beam2.pos = { boss.pos.x,boss.pos.y };
		} else {
			beam2.isAttak = true;
		}

		if (beam2.isAttak) {
			if (beam2.attakTime > 0) {
				beam2.attakTime--;
				boss.pos.x += beam2.speed;
				beam2.pos.x += beam2.speed;

				if (boss.pos.x > 1280.0f - boss.size.x || boss.pos.x < 0) {
					beam2.speed *= -1;
				}
				shake.pos.x = static_cast<float>(rand() % 20 - 10);
				shake.pos.y = static_cast<float>(rand() % 20 - 10);
			} else {
				Vector2 endPos = { 1000.0f,472.0f };
				boss.pos.x += (endPos.x - boss.pos.x) * 0.05f;
				boss.pos.y += (endPos.y - boss.pos.y) * 0.05f;
				shake.pos.x = 0.0f;
				shake.pos.y = 0.0f;
				boss.vanishTime = 2;

				if (std::abs(endPos.x - boss.pos.x) < 0.5f && std::abs(endPos.y - boss.pos.y) < 0.5f) {
					boss.pos = endPos;
					beam2.isEase = false;
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120; // クールダウン

				}
			}
		}
	}


}

// チャージ攻撃の描画
void Boss::DrawBossChargeAttak(const Object& object) {

	if (object.isAttak) {
		Novice::DrawBox(static_cast<int>(object.pos.x), static_cast<int>(object.pos.y),
			static_cast<int>(object.size.x), static_cast<int>(object.size.y),
			0.0f, RED, kFillModeSolid);


	}
}

//飛び道具
void Boss::UpdateProjectiles(Projectile* projectiles) {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].isActive) {
			projectiles[i].pos.x += projectiles[i].velocity.x;
			projectiles[i].pos.y += projectiles[i].velocity.y;
			if (projectiles[i].pos.x < 0 || projectiles[i].pos.x > 1280 ||
				projectiles[i].pos.y < 0 || projectiles[i].pos.y > 720) {
				projectiles[i].isActive = false;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if (projectiles[i].isActive) {
			// 球として描画
			Novice::DrawEllipse(
				static_cast<int>(projectiles[i].pos.x),
				static_cast<int>(projectiles[i].pos.y),
				10, 10, // 半径
				0.0f,
				WHITE,
				kFillModeSolid
			);
		}
	}
}


void Boss::DrawWhole() {
	for (int i = 0; i < MAX_boll; i++) {
		if (bossData.bullets[i].isAttak) {
			// デバッグ用ログ (必要なら表示を確認)
			Novice::ScreenPrintf(0, 20 * i, "Bullet[%d]: x=%.2f, y=%.2f, active=%d",
				i, bossData.bullets[i].pos.x, bossData.bullets[i].pos.y, bossData.bullets[i].isAttak);

			Novice::DrawBox(
				static_cast<int>(bossData.bullets[i].pos.x) - 5,
				static_cast<int>(bossData.bullets[i].pos.y) - 5,
				10, 10, 0.0f, RED, kFillModeSolid);
		}
	}
}


//=========================
//当たり判定の作成
//=========================

//範囲攻撃
void Boss::PlayerRengeHitBox(Player_& player, BossRengeAttak_& renge) {
	player.isHit = false;

	// プレイヤーと範囲攻撃の矩形が重なっているかチェック
	if (renge.attakStandTime <= 0) {
		if (renge.attakTime > 0) {
			if (player.pos.x < renge.pos.x + renge.size.x && // プレイヤーの右端が範囲の左端より右
				player.pos.x + player.radius > renge.pos.x && // プレイヤーの左端が範囲の右端より左
				player.pos.y < renge.pos.y + renge.size.y && // プレイヤーの下端が範囲の上端より下
				player.pos.y + player.radius > renge.pos.y) { // プレイヤーの上端が範囲の下端より上
				player.isHit = true;
			}
		}
	}
}

//近距離攻撃
void Boss::PlayerShortAttakHitBox(Player_& player, ShortDistansAttak_& shortAttak) {
	player.isHit = false;

	// プレイヤーと範囲攻撃の矩形が重なっているかチェック
	if (!shortAttak.isHit) {
		if (shortAttak.attakTime > 0) {
			if (player.pos.x < shortAttak.pos.x + shortAttak.size.x && // プレイヤーの右端が範囲の左端より右
				player.pos.x + player.radius > shortAttak.pos.x && // プレイヤーの左端が範囲の右端より左
				player.pos.y < shortAttak.pos.y + shortAttak.size.y && // プレイヤーの下端が範囲の上端より下
				player.pos.y + player.radius > shortAttak.pos.y) { // プレイヤーの上端が範囲の下端より上
				player.isHit = true;

			}
		}
	}
}

//ビームとボスの当たり判定
void Boss::PlayerBeamsHitBox(Player_& player, Boss_& boss) {
	player.isHit = false;
	for (int i = 0; i < MAX_BEAMS; i++) {
		if (boss.beams[i].attakStandTime <= 0) {
			if (boss.beams[i].attakTime > 0) {
				if (player.pos.x < boss.beams[i].pos.x + boss.beams[i].size.x && // プレイヤーの右端が範囲の左端より右
					player.pos.x + player.radius > boss.beams[i].pos.x && // プレイヤーの左端が範囲の右端より左
					player.pos.y < boss.beams[i].pos.y + boss.beams[i].size.y && // プレイヤーの下端が範囲の上端より下
					player.pos.y + player.radius > boss.beams[i].pos.y) { // プレイヤーの上端が範囲の下端より上

					player.isHit = true;
				}
			}
		}
	}
}

void Boss::DoubleShortDistansAttak(Boss_& boss, ShortDubleDistansAttak_& shortDobleDist, Player_ player) {
	shortDobleDist.isAttak = true;

	if (shortDobleDist.isAttak) {
		if (shortDobleDist.attakCount == 0) {
			// 最初の攻撃: プレイヤーにイージングで近づく
			float distanceX = player.pos.x - boss.pos.x;
			float stopDistance = 100.0f; // プレイヤーからこの距離で停止

			if (std::abs(distanceX) > stopDistance) {
				// プレイヤーに近づく
				if (shortDobleDist.isEase) {
					boss.pos.x += distanceX * shortDobleDist.easeSpeed;
				}
				shortDobleDist.attakTime = 10; // 攻撃判定をリセット
				shortDobleDist.isShortAttak = false;
			} else {
				// 一定距離まで近づいたら攻撃
				if (shortDobleDist.attakTime == 10) {
					if (player.pos.x < boss.pos.x) {
						shortDobleDist.pos.x = boss.pos.x - shortDobleDist.size.x;
						shortDobleDist.expandDirection = -1; // 左方向
					} else {
						shortDobleDist.pos.x = boss.pos.x + boss.size.x;
						shortDobleDist.expandDirection = 1;  // 右方向
					}
					shortDobleDist.pos.y = boss.pos.y + boss.size.y / 2 - shortDobleDist.size.y / 2;
				}

				if (shortDobleDist.attakTime > 0) {
					shortDobleDist.isShortAttak = true;
					shortDobleDist.attakTime--;
				} else {
					shortDobleDist.attakCount++;
					shortDobleDist.isHit = false;
					shortDobleDist.attakTime = 10; // 次回攻撃の準備
				}
			}

		} else if (shortDobleDist.attakCount == 1) {
			// 2回目の攻撃: 横方向に攻撃範囲を拡大
			if (shortDobleDist.attakTime == 10) {
				shortDobleDist.size.x = shortDobleDist.baseSizeX; // サイズをリセットして拡大開始
			}

			if (shortDobleDist.size.x < shortDobleDist.maxExpandSize) {
				shortDobleDist.size.x += 20.0f; // 攻撃範囲を拡大
				if (shortDobleDist.expandDirection == -1) {
					// 左方向に拡大
					shortDobleDist.pos.x = boss.pos.x - shortDobleDist.size.x;
				} else {
					// 右方向に拡大
					shortDobleDist.pos.x = boss.pos.x + boss.size.x;
				}
			}

			if (shortDobleDist.attakTime > 0) {
				shortDobleDist.isShortAttak = true;
				shortDobleDist.attakTime--;
			} else {
				// 攻撃終了
				shortDobleDist.isHit = false;
				shortDobleDist.isShortAttak = false;
				shortDobleDist.isAttak = false;
				shortDobleDist.attakCount = 0;
				shortDobleDist.size.x = shortDobleDist.baseSizeX; // サイズをリセット
				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120; // クールダウン
			}
		}
	}
}

//連続攻撃
void Boss::PlayerShortDobleAttakHitBox(Player_& player, ShortDubleDistansAttak_& doubleAttak) {
	player.isHit = false;
	if (doubleAttak.isShortAttak) {
		if (doubleAttak.attakTime > 0) {
			if (player.pos.x < doubleAttak.pos.x + doubleAttak.size.x && // プレイヤーの右端が範囲の左端より右
				player.pos.x + player.radius > doubleAttak.pos.x && // プレイヤーの左端が範囲の右端より左
				player.pos.y < doubleAttak.pos.y + doubleAttak.size.y && // プレイヤーの下端が範囲の上端より下
				player.pos.y + player.radius > doubleAttak.pos.y) { // プレイヤーの上端が範囲の下端より上

				player.isHit = true;

				doubleAttak.isHit = true;

			}
		}
	}
}

//void InitializeAllRangeAttack(AllRange* beams) {
//	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
//		beams[i].startPos = { static_cast<float>(rand() % 1280),  static_cast<float>(rand() % 720) }; // ランダムな開始位置
//		beams[i].endPos = { static_cast<float>(rand() % 1280),  static_cast<float>(rand() % 720) };   // ランダムな終了位置
//		beams[i].currentPos = beams[i].startPos;
//
//		// 進行方向を計算
//		beams[i].direction = {
//			beams[i].endPos.x - beams[i].startPos.x,
//			beams[i].endPos.y - beams[i].startPos.y
//		};
//
//		// 正規化して速度を掛ける
//		float length = std::sqrt(beams[i].direction.x * beams[i].direction.x + beams[i].direction.y * beams[i].direction.y);
//		beams[i].direction.x /= length;
//		beams[i].direction.y /= length;
//		beams[i].speed = 5.0f;
//
//		beams[i].isActive = 1;
//		beams[i].lifeTime = 120; // 寿命は2秒（60FPS）
//	}
//}


//===================
//第二形態の処理
//===================

//第2形態のビーム
void Boss::DrawBeam2(Beam2& beam2) {
	if (beam2.attakStandTime <= 0) {
		if (beam2.attakTime > 0) {
			if (beam2.attakTime % 2 == 0) {
				Novice::DrawBox(static_cast<int>(beam2.pos.x), static_cast<int>(beam2.pos.y),
					static_cast<int>(beam2.size.x), static_cast<int>(beam2.size.y), 0.0f, RED, kFillModeSolid);
			}
		}
	}
}

//全体攻撃
void Boss::InitializeAllRangeAttack(Boss_& allRange) {
	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
		// ビームの開始位置を画面上部にランダム配置
		allRange.allRangeBeams[i].startPos = {
			static_cast<float>(rand() % 1280), // 横方向のランダムな位置
			0.0f                              // 天井（画面上部）
		};

		// ビームの終了位置を画面下部にランダム配置
		allRange.allRangeBeams[i].endPos = {
			static_cast<float>(rand() % 1280), // 横方向のランダムな位置
			720.0f                            // 地面（画面下部）
		};

		allRange.allRangeBeams[i].currentPos = allRange.allRangeBeams[i].startPos;

		// 進行方向を計算
		allRange.allRangeBeams[i].direction = {
			allRange.allRangeBeams[i].endPos.x - allRange.allRangeBeams[i].startPos.x,
			allRange.allRangeBeams[i].endPos.y - allRange.allRangeBeams[i].startPos.y
		};

		// 正規化して進行方向を単位ベクトルにする
		float length = std::sqrt(
			allRange.allRangeBeams[i].direction.x * allRange.allRangeBeams[i].direction.x +
			allRange.allRangeBeams[i].direction.y * allRange.allRangeBeams[i].direction.y
		);

		if (length != 0.0f) {
			allRange.allRangeBeams[i].direction.x /= length;
			allRange.allRangeBeams[i].direction.y /= length;
		}

		// ビームの初期パラメータ設定
		allRange.allRangeBeams[i].speed = 5.0f; // 移動速度
		allRange.allRangeBeams[i].isActive = 1; // ビームをアクティブ化
		allRange.allRangeBeams[i].lifeTime = 120; // 寿命は2秒（60FPS）
	}
}


//全体攻撃
void Boss::UpdateAllRangeAttack(Boss_& allRange) {
	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
		if (allRange.allRangeBeams[i].isActive) {
			// 現在位置を更新
			allRange.allRangeBeams[i].currentPos.x += allRange.allRangeBeams[i].direction.x * allRange.allRangeBeams[i].speed;
			allRange.allRangeBeams[i].currentPos.y += allRange.allRangeBeams[i].direction.y * allRange.allRangeBeams[i].speed;

			// 寿命を減らす
			allRange.allRangeBeams[i].lifeTime--;

			// 寿命切れまたは目標地点に到達したら無効化
			if (allRange.allRangeBeams[i].lifeTime <= 0 ||
				(std::abs(allRange.allRangeBeams[i].currentPos.x - allRange.allRangeBeams[i].endPos.x) < 5.0f &&
					std::abs(allRange.allRangeBeams[i].currentPos.y - allRange.allRangeBeams[i].endPos.y) < 5.0f)) {
				allRange.allRangeBeams[i].isActive = 0;
			}
		}
	}
}

//全体攻撃の描画
void Boss::DrawAllRangeAttack(Boss_& allRange) {
	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
		if (allRange.allRangeBeams[i].isActive) {
			// 残りフレームが10以下の場合は太い線を描画

			if (allRange.allRangeBeams[i].lifeTime >= 11 && allRange.allRangeBeams[i].lifeTime <= 12) {
				Novice::DrawBox(0, 0, 1280, 720, 0.0f, WHITE, kFillModeSolid);
			}

			if (allRange.allRangeBeams[i].lifeTime <= 10) {
				// 線を太く描画する
				float thickness = 10.0f; // 線の太さ
				Vector2 direction = {
					allRange.allRangeBeams[i].endPos.x - allRange.allRangeBeams[i].startPos.x,
					allRange.allRangeBeams[i].endPos.y - allRange.allRangeBeams[i].startPos.y
				};

				// 線の長さを計算
				float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

				// 正規化
				if (length != 0.0f) {
					direction.x /= length;
					direction.y /= length;
				}

				// 垂直方向のベクトルを計算（線の幅を表現するため）
				Vector2 perpendicular = { -direction.y * thickness, direction.x * thickness };

				// 線を四角形として描画
				Novice::DrawBox(
					static_cast<int>(allRange.allRangeBeams[i].startPos.x - perpendicular.x / 2),
					static_cast<int>(allRange.allRangeBeams[i].startPos.y - perpendicular.y / 2),
					static_cast<int>(length), // 線の長さ
					static_cast<int>(thickness), // 線の太さ
					std::atan2(direction.y, direction.x), // 線の回転角度
					WHITE, // 色
					kFillModeSolid // 塗りつぶし
				);
			} else {
				// 通常の細い線を描画
				Novice::DrawLine(
					static_cast<int>(allRange.allRangeBeams[i].startPos.x),
					static_cast<int>(allRange.allRangeBeams[i].startPos.y),
					static_cast<int>(allRange.allRangeBeams[i].endPos.x),
					static_cast<int>(allRange.allRangeBeams[i].endPos.y),
					WHITE
				);
			}
		}
	}
}


// 全体攻撃の当たり判定
void Boss::AllRengeAttakHitBox(Boss_& allRange, Player_& player) {

	for (int i = 0; i < MAX_ALLRANGE_BEAMS; i++) {
		if (allRange.allRangeBeams[i].isActive) {
			// ビームの開始位置と終了位置
			Vector2 start = allRange.allRangeBeams[i].startPos;
			Vector2 end = allRange.allRangeBeams[i].endPos;

			// プレイヤーの位置
			Vector2 playerPos = player.pos;

			// ビームのベクトル
			Vector2 beamVec = { end.x - start.x, end.y - start.y };

			// プレイヤーからビームの開始点へのベクトル
			Vector2 playerVec = { playerPos.x - start.x, playerPos.y - start.y };

			// ビームの長さの2乗を計算
			float beamLengthSquared = beamVec.x * beamVec.x + beamVec.y * beamVec.y;

			// t を計算（0.0f～1.0fの範囲に収める）
			float t = (beamVec.x * playerVec.x + beamVec.y * playerVec.y) / beamLengthSquared;
			if (t < 0.0f) {
				t = 0.0f;
			} else if (t > 1.0f) {
				t = 1.0f;
			}

			// 最近接点を計算
			Vector2 closestPoint = {
				start.x + t * beamVec.x,
				start.y + t * beamVec.y
			};

			// プレイヤーとの距離を計算
			float distanceX = playerPos.x - closestPoint.x;
			float distanceY = playerPos.y - closestPoint.y;
			float distanceSquared = distanceX * distanceX + distanceY * distanceY;

			// 当たり判定の閾値（プレイヤーの半径やビームの幅）
			float hitRadius = player.radius;

			// プレイヤーが当たり判定内にいるかチェック
			if (distanceSquared <= hitRadius * hitRadius) {
				player.isHit = true;
				break; // 一つのビームでも当たれば処理を終了
			}
		}
	}
}

//落下攻撃の処理
void Boss::BossFallAttak(Boss_& boss, Player_& player, Shake& shake, Shockwave* shockwaves, int maxShockwaves) {
	Vector2 easePos = { 640.0f, 100.0f };

	if (boss.fallCoolTime == 0) {
		if (boss.fallCount < 4) {
			if (!boss.isFallAttak) {
				boss.pos.x += (easePos.x - boss.pos.x) * 0.05f;
				boss.pos.y += (easePos.y - boss.pos.y) * 0.05f;
				boss.vanishTime = 2;

				if (std::fabs(easePos.x - boss.pos.x) < 0.5f && std::fabs(easePos.y - boss.pos.y) < 0.5f) {
					boss.pos = easePos;
					boss.isFallAttak = true;
				}
			} else {
				if (!boss.isFall) {
					if (std::fabs(boss.pos.x - player.pos.x) < 10.0f) {
						boss.isFall = true;
					} else {
						boss.pos.x += boss.speed;

						if (boss.pos.x <= 0 || boss.pos.x > 1280.0f - boss.size.x) {
							boss.speed *= -1;
						}
					}
				} else {
					if (boss.pos.y > 600.0f - boss.size.y) {
						boss.fallCoolTime = 40;
						boss.fallCount++;
						boss.isFall = false;
						boss.isFallAttak = false;
						shake.time = 20;

						// 衝撃波を生成
						int createdShockwaves = 0; // 生成済み衝撃波の数
						for (int i = 0; i < maxShockwaves; i++) {
							if (!shockwaves[i].isActive) {
								if (createdShockwaves == 0) {
									// 左側の衝撃波
									shockwaves[i].pos = { boss.pos.x, 600.0f - boss.size.y / 2 };
									shockwaves[i].size = { 32.0f, 32.0f };
									shockwaves[i].direction = { -1.0f, 0.0f }; // 左方向
									shockwaves[i].speed = 10.0f;
									shockwaves[i].isActive = 1;
									createdShockwaves++;
								} else if (createdShockwaves == 1) {
									// 右側の衝撃波
									shockwaves[i].pos = { boss.pos.x + boss.size.x, 600.0f - boss.size.y / 2 };
									shockwaves[i].size = { 32.0f, 32.0f };
									shockwaves[i].direction = { 1.0f, 0.0f }; // 右方向
									shockwaves[i].speed = 10.0f;
									shockwaves[i].isActive = 1;
									break;
								}
							}
						}
					} else {
						boss.pos.y += boss.fallSpeed;
					}
				}
			}
		} else {
			Vector2 firstPos = { 1000.0f, 478.0f };
			boss.pos.x += (firstPos.x - boss.pos.x) * 0.05f;
			boss.pos.y += (firstPos.y - boss.pos.y) * 0.05f;
			boss.vanishTime = 2;

			if (std::fabs(firstPos.x - boss.pos.x) < 0.5f && std::fabs(firstPos.y - boss.pos.y) < 0.5f) {
				boss.pos = firstPos;

				boss.isAttak = false;
				boss.attakNo = 0;
				boss.attakStandTime = 120; // クールダウン
			}
		}
	} else if (boss.fallCoolTime > 0) {
		boss.fallCoolTime--;
	}

	if (shake.time > 0) {
		shake.time--;
	}

	if (shake.time > 5) {
		shake.bgPos.x = static_cast<float>(rand() % shake.time - 5);
		shake.bgPos.y = static_cast<float>(rand() % shake.time - 5);
	} else {
		shake.bgPos.x = 0.0f;
		shake.bgPos.y = 0.0f;
	}
}

void Boss::UpdateShockwaves(Shockwave* shockwaves, int maxShockwaves) {
	for (int i = 0; i < maxShockwaves; i++) { // maxShockwavesを使ってループの上限を設定
		if (shockwaves[i].isActive) {
			shockwaves[i].pos.x += shockwaves[i].direction.x * shockwaves[i].speed;

			// 衝撃波が画面外に出たら無効化
			if (shockwaves[i].pos.x < 0 || shockwaves[i].pos.x > 1280) {
				shockwaves[i].isActive = 0;
			}
		}
	}
}


void Boss::DrawShockwaves(Shockwave* shockwaves, int maxShockwaves) {
	for (int i = 0; i < maxShockwaves; i++) { // maxShockwavesを使ってループの上限を設定
		if (shockwaves[i].isActive) {
			Novice::DrawBox(
				static_cast<int>(shockwaves[i].pos.x),
				static_cast<int>(shockwaves[i].pos.y),
				static_cast<int>(shockwaves[i].size.x),
				static_cast<int>(shockwaves[i].size.y),
				0.0f,
				RED,
				kFillModeSolid
			);
		}
	}
}

//=========================
//ワープ攻撃
//=========================
// ワープ攻撃の処理（ランダム攻撃付き）
// ワープ攻撃の処理
void Boss::BossWarpAttak(Boss_& boss, Player_& player, WarpAttak& warp, ShortDistansAttak_& shortDist, Shake& shake) {
	// ワープ攻撃が始まる条件
	if (!warp.isAttak) {
		// プレイヤーの向きに応じてワープ位置を変更
		if (player.isRight) { // プレイヤーが右を向いている場合
			warp.pos.x = player.pos.x - 50.0f; // プレイヤーの左後ろにワープ
		} else if (player.isLeft) { // プレイヤーが左を向いている場合
			warp.pos.x = player.pos.x + 150.0f; // プレイヤーの右後ろにワープ
		}
		warp.pos.y = player.pos.y;

		warp.attakTime = 60;  // ワープ後の攻撃時間を設定
		boss.pos = warp.pos; // ボスをワープさせる
		warp.isAttak = true; // ワープ攻撃を開始

		if (player.isRight) { // プレイヤーが右向き
			shortDist.pos = { boss.pos.x+75.0f , boss.pos.y + boss.size.y / 2 - shortDist.size.y / 2 };
		} else if (player.isLeft) { // プレイヤーが左向き
			shortDist.pos = { boss.pos.x - boss.size.x-50.0f, boss.pos.y + boss.size.y / 2 - shortDist.size.y / 2 };
		} 
	}

	// 攻撃時間中は近接攻撃
	if (warp.isAttak && warp.attakTime > 0) {
		warp.attakTime--;

		// 攻撃位置を設定（プレイヤーの向きに応じた調整）
		if (warp.attakTime > 0 && warp.attakTime < 40) {
			shortDist.isAttak = true;
			player.isHit = false;
			// 画面を揺らす演出
			shake.bossPos.x = static_cast<float>(rand() % 41 - 20);
			shake.bossPos.y = static_cast<float>(rand() % 41 - 20);

			if (player.pos.x < shortDist.pos.x + shortDist.size.x && // プレイヤーの右端が範囲の左端より右
				player.pos.x + player.radius > shortDist.pos.x && // プレイヤーの左端が範囲の右端より左
				player.pos.y < shortDist.pos.y + shortDist.size.y && // プレイヤーの下端が範囲の上端より下
				player.pos.y + player.radius > shortDist.pos.y) { // プレイヤーの上端が範囲の下端より上
				player.isHit = true;
			}
		} else if (warp.attakTime >= 40) {
			// ワープ直後の演出
			shake.bossPos.x = static_cast<float>(rand() % 21 - 10);
			shake.bossPos.y = static_cast<float>(rand() % 21 - 10);
		}
	} else if (warp.attakTime <= 0) {
		// 攻撃終了後のリセット
		warp.isAttak = false;
		boss.isAttak = false;
		boss.attakNo = 0;
		boss.attakStandTime = 120; // クールダウン時間
	}
}


// ワープ攻撃の描画
void Boss::DrawWarpAttak(WarpAttak& warp, ShortDistansAttak_& shortDist) {
	if (warp.isAttak) {
		if (shortDist.isAttak) {
			if (warp.attakTime > 0 && warp.attakTime < 40) {
				Novice::DrawBox(
					static_cast<int>(shortDist.pos.x),
					static_cast<int>(shortDist.pos.y),
					static_cast<int>(shortDist.size.x),
					static_cast<int>(shortDist.size.y), // エフェクトのサイズ
					0.0f,
					BLUE, // 緑色
					kFillModeSolid
				);
			}
		}
	}
}

//if (boss.attakNo == 3) {
//	object.isAttak = true;
//
//	object.timer++;
//
//	if (object.isAttak) {
//		// タイマーの進行
//		object.timer++;
//
//		// 浮遊状態と飛ばす動きの切り替え
//		if (object.timer >= 300) { // 5秒後
//			object.isFloating = false;
//		}
//
//		// 浮遊状態の処理
//		if (object.isFloating) {
//			// ボスの周囲を回転
//			object.rotation += 0.05f; // 回転速度
//			object.pos.x = boss.pos.x + object.orbitRadius * cos(object.rotation);
//			object.pos.y = boss.pos.y + object.orbitRadius * sin(object.rotation);
//		} else {
//			// プレイヤーに向かって飛ばす処理
//
//			Vector2 direction = { player.pos.x - object.pos.x, player.pos.y - object.pos.y };
//			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//
//			if (length != 0) {
//				// ベクトルの正規化
//				direction.x /= length;
//				direction.y /= length;
//			}
//
//			// チャージオブジェクトの速度を適用して移動
//			object.pos.x += direction.x * object.throwSpeed;
//			object.pos.y += direction.y * object.throwSpeed;
//
//			// プレイヤーに接近したら攻撃終了
//			float stopThreshold = 50.0f; // 近接判定のしきい値
//			if (length < stopThreshold) {
//				// 攻撃終了処理
//				object.isAttak = false;
//				boss.isAttak = false;
//				boss.attakNo = 0;
//				boss.attakStandTime = 120; // クールダウン時間
//				object.timer = 0;
//				object.timer = 0;
//				object.rotation = 0.0f;
//				object.throwSpeed = 15.0f; // 初期スピードに戻す
//				object.pos.x = boss.pos.x + object.orbitRadius;
//				object.pos.y = boss.pos.y + object.orbitRadius;
//				object.isAttak = false;
//				object.isFloating = true;
//				boss.isAttak = false;
//				boss.attakNo = 0;
//				boss.attakStandTime = 120;  // 攻撃後のクールダウン
//				object.attakTime = 360;
//			}
//
//
//
//
//		}
//	}
//}