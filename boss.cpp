
#include "Novice.h"
#include "Data.h"
#include "boss.h"
#include <cmath> // 距離計算のために必要

int timer = 0;  // timer をここで定義

int globalTimer = 0;  // グローバル変数の名前を変更
// ボスのイージング移動処理
void Boss::BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed) {
	boss.pos.x += (targetPos.x - boss.pos.x) * easingSpeed;
	boss.pos.y += (targetPos.y - boss.pos.y) * easingSpeed;

	if (std::abs(targetPos.x - boss.pos.x) < 0.5f && std::abs(targetPos.y - boss.pos.y) < 0.5f) {
		boss.pos = targetPos;
	}
}


// ボスの動き
void Boss::BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player, Object& object, ShortDubleDistansAttak_& doubleShort, Shake& shake, Beam2& beam2, Projectile* projectiles, int& localTimer) {
	// 関数内容



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
					boss.attakNo = 5;
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
		PlayerRengeHitBox(player, renge);
		Novice::ScreenPrintf(200, 500, "%d", player.isHit);
	}



	// 近距離攻撃処理
	if (boss.attakNo == 2) {
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

		PlayerShortAttakHitBox(player, shortDist);
		Novice::ScreenPrintf(200, 500, "%d", player.isHit);

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
		doubleShort.isAttak = true;

		if (doubleShort.isAttak) {
			if (doubleShort.attakCount == 0) {
				// 最初の攻撃: プレイヤーにイージングで近づく
				float distanceX = player.pos.x - boss.pos.x;
				float stopDistance = 100.0f; // プレイヤーからこの距離で停止

				if (std::abs(distanceX) > stopDistance) {
					// プレイヤーに近づく
					if (doubleShort.isEase) {
						boss.pos.x += distanceX * doubleShort.easeSpeed;
					}
					doubleShort.attakTime = 10; // 攻撃判定をリセット
					doubleShort.isShortAttak = false;
				} else {
					// 一定距離まで近づいたら攻撃
					if (doubleShort.attakTime == 10) {
						if (player.pos.x < boss.pos.x) {
							doubleShort.pos.x = boss.pos.x - doubleShort.size.x;
							doubleShort.expandDirection = -1; // 左方向
						} else {
							doubleShort.pos.x = boss.pos.x + boss.size.x;
							doubleShort.expandDirection = 1;  // 右方向
						}
						doubleShort.pos.y = boss.pos.y + boss.size.y / 2 - doubleShort.size.y / 2;
					}

					if (doubleShort.attakTime > 0) {
						doubleShort.isShortAttak = true;
						doubleShort.attakTime--;
					} else {
						doubleShort.attakCount++;
						doubleShort.isHit = false;
						doubleShort.attakTime = 10; // 次回攻撃の準備
					}
				}

			} else if (doubleShort.attakCount == 1) {
				// 2回目の攻撃: 横方向に攻撃範囲を拡大
				if (doubleShort.attakTime == 10) {
					doubleShort.size.x = doubleShort.baseSizeX; // サイズをリセットして拡大開始
				}

				if (doubleShort.size.x < doubleShort.maxExpandSize) {
					doubleShort.size.x += 20.0f; // 攻撃範囲を拡大
					if (doubleShort.expandDirection == -1) {
						// 左方向に拡大
						doubleShort.pos.x = boss.pos.x - doubleShort.size.x;
					} else {
						// 右方向に拡大
						doubleShort.pos.x = boss.pos.x + boss.size.x;
					}
				}

				if (doubleShort.attakTime > 0) {
					doubleShort.isShortAttak = true;
					doubleShort.attakTime--;
				} else {
					// 攻撃終了
					doubleShort.isHit = false;
					doubleShort.isShortAttak = false;
					doubleShort.isAttak = false;
					doubleShort.attakCount = 0;
					doubleShort.size.x = doubleShort.baseSizeX; // サイズをリセット
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120; // クールダウン
				}
			}
		}
		PlayerShortDobleAttakHitBox(player, doubleShort);
		Novice::ScreenPrintf(200, 500, "%d", player.isHit);
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




	if (boss.attakNo == 3) {
		object.isAttak = true;
		if (object.isAttak) {
			// タイマーの進行
			timer++;

			// 浮遊状態と飛ばす動きの切り替え
			if (timer >= 300) { // 5秒後
				object.isFloating = false;
			}

			// 浮遊状態の処理
			if (object.isFloating) {
				// ボスの周囲を回転
				object.rotation += 0.05f; // 回転速度
				object.pos.x = boss.pos.x + object.orbitRadius * cos(object.rotation);
				object.pos.y = boss.pos.y + object.orbitRadius * sin(object.rotation);
			} else {
				// プレイヤーに向かって飛ばす処理
				if (!object.isFloating) {
					// プレイヤーの方向ベクトルを計算
					Vector2 direction = { player.pos.x - object.pos.x, player.pos.y - object.pos.y };
					float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

					if (length != 0) {
						// ベクトルの正規化
						direction.x /= length;
						direction.y /= length;
					}

					// チャージオブジェクトの速度を適用して移動
					object.pos.x += direction.x * object.throwSpeed;
					object.pos.y += direction.y * object.throwSpeed;

					// プレイヤーに接近したら攻撃終了
					float stopThreshold = 50.0f; // 近接判定のしきい値
					if (length < stopThreshold) {
						// 攻撃終了処理
						object.isAttak = false;
						boss.isAttak = false;
						boss.attakNo = 0;
						boss.attakStandTime = 120; // クールダウン時間
						timer = 0;
					}


					timer = 0;
					object.rotation = 0.0f;
					object.throwSpeed = 15.0f; // 初期スピードに戻す
					object.pos.x = boss.pos.x + object.orbitRadius;
					object.pos.y = boss.pos.y + object.orbitRadius;
					object.isAttak = false;
					object.isFloating = true;
					boss.isAttak = false;
					boss.attakNo = 0;
					boss.attakStandTime = 120;  // 攻撃後のクールダウン
					object.attakTime = 360;
				}
			}
		}
}




	if (boss.attakNo == 10) {
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
	Novice::ScreenPrintf(32, 352, "attakStandTime: %d", beam2.attakStandTime);
	Novice::ScreenPrintf(32, 384, "attakTime: %d", beam2.attakTime);
	Novice::ScreenPrintf(32, 416, "pos.x: %.1f, pos.y: %.1f", beam2.pos.x, beam2.pos.y);

	localTimer = 0;
	if (!boss.isFloating) {
		if (localTimer >= 5) {
			boss.isFloating = true;
			localTimer = 0;
		}
	} else {
		if (boss.pos.y > 400.0f && boss.attackCount < MAX_PROJECTILES) {
			boss.pos.y -= 2.0f;
		} else if (boss.attackCount < MAX_PROJECTILES) {
			if (timer >= 20) {
				projectiles[boss.attackCount].pos = boss.pos;
				Vector2 direction = { player.pos.x - boss.pos.x, player.pos.y - boss.pos.y };
				float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				direction.x /= length;
				direction.y /= length;

				projectiles[boss.attackCount].velocity = { direction.x * 8.0f, direction.y * 8.0f };
				projectiles[boss.attackCount].isActive = true;
				boss.attackCount++;
				timer = 0;
			}
		} else {
			if (boss.pos.y < 600.0f) {
				boss.pos.y += 2.0f;
			} else {
				boss.isFloating = false;
				boss.attackCount = 0;
			}
		}
	}
}

// ボスを描画する
void Boss::BossDraw(Boss_ boss, Shake& shake) {
	/*Novice::DrawBox(static_cast<int>(boss.pos.x), static_cast<int>(boss.pos.y),
		static_cast<int>(boss.size.x), static_cast<int>(boss.size.y), 0.0f, WHITE, kFillModeSolid);*/
	if (boss.vanishTime == 0) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x), static_cast<int>(boss.pos.y + shake.pos.y),
			boss.image, 1, 1, 0.0f, WHITE);
	} else if (boss.vanishTime < 10) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x), static_cast<int>(boss.pos.y + shake.pos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF50);
	} else if (20 > boss.vanishTime && boss.vanishTime > 10) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x), static_cast<int>(boss.pos.y + shake.pos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF40);
	} else if (30 > boss.vanishTime && boss.vanishTime > 20) {
		Novice::DrawSprite(static_cast<int>(boss.pos.x + shake.pos.x), static_cast<int>(boss.pos.y + shake.pos.y),
			boss.image, 1, 1, 0.0f, 0xFFFFFF30);
	}
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
	if (object.isAttak) {
		Novice::DrawBox(
			static_cast<int>(object.pos.x) - 50,
			static_cast<int>(object.pos.y) - 50,
			100,
			100,
			0.0f,
			WHITE,
			kFillModeSolid
		);

	}
}

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



//===================
//第二形態の処理
//===================

//第2形態のビーム
void Boss::DrawBeam2(Beam2& beam2) {
	if (beam2.attakStandTime <= 0) {
		if (beam2.attakTime > 0) {
			Novice::DrawBox(static_cast<int>(beam2.pos.x), static_cast<int>(beam2.pos.y),
				static_cast<int>(beam2.size.x), static_cast<int>(beam2.size.y), 0.0f, RED, kFillModeSolid);
		}
	}
}

