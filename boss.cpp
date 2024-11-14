#include "Novice.h"
#include "Data.h"
#include "boss.h"

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
            boss.attakNo = 1;  // 範囲攻撃
             //boss.attakNo = 2; //近距離攻撃
        }
    }

    // 攻撃番号が1の場合、レンジ攻撃を開始
    if (boss.attakNo == 1) {
        renge.isAttak = true;

        if (renge.isAttak) {
            // standTimeを減少
            if (renge.attakStandTime > 0) {
                renge.attakStandTime--;
                renge.attakTime = 90;
                renge.pos.x = boss.pos.x - 256.0f + 32.0f;  // ボスの位置に基づいて攻撃位置を設定
                renge.pos.y = boss.pos.y - 500.0f - 128.0f + 56.0f;
            } else {
                // standTimeが0になったらattakTimeを減少
                if (renge.attakTime > 0) {
                    renge.attakTime--;
                } else {
                    // 攻撃終了、フラグをリセット
                    renge.isAttak = false;
                    boss.isAttak = false;
                    boss.attakNo = 0;
                    boss.attakStandTime = 120;  // 次の攻撃までの待機時間をリセット
                    renge.attakStandTime = 120;  // 次の攻撃準備時間をリセット
                }
            }
        }
    }

    // 近距離攻撃処理
    if (boss.attakNo == 2) {
        shortDist.isAttak = true;
        if (shortDist.isAttak) {
            // プレイヤーに向かってイージングで移動
            boss.pos.x += (player.pos.x - boss.pos.x) * shortDist.easeSpeed;
            boss.pos.y += (player.pos.y - boss.pos.y) * shortDist.easeSpeed;

            if (shortDist.attakTime > 0) {
                shortDist.attakTime--;
            } else {
                shortDist.isAttak = false;
                boss.isAttak = false;
                boss.attakNo = 0;
                boss.attakStandTime = 120;  // 攻撃後のクールダウン
                shortDist.attakTime = 90;
            }
        }
    }

    // デバッグ用出力
    Novice::ScreenPrintf(32, 32, "attakNo::%d", boss.attakNo);
    Novice::ScreenPrintf(32, 64, "bossStandTime::%d", boss.attakStandTime);
    Novice::ScreenPrintf(32, 96, "rengeStandTime::%d", renge.attakStandTime);
    Novice::ScreenPrintf(32, 128, "rengeAttakTime::%d", renge.attakTime);
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
