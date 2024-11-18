#include <Novice.h>
#include <math.h>
#include "Data.h"
#include "Player.h"
#include "boss.h"

const char kWindowTitle[] = "1221_霊障退治";

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
    Boss boss;
    Boss_ boss_;
    BossRengeAttak_ rengeAttak_;
    ShortDistansAttak_ shortDistAttak_;  // 近距離攻撃用

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        // プレイヤーの更新処理
        player.Move(player_, line, keys, preKeys);

        // ボスの更新処理（範囲攻撃と近距離攻撃を含む）
        boss.BossMove(boss_, rengeAttak_, shortDistAttak_, player_);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // プレイヤーの描画
        player.Draw(player_);

        // ボスの範囲攻撃描画
        boss.DrawBossRengeAttak(rengeAttak_);

        // ボスの描画
        boss.BossDraw(boss_);

        //近距離攻撃の描画
        boss.DrawShortDistansAttak(shortDistAttak_);

        // ビーム攻撃の描画
        boss.DrawBeams(boss_);

        ///
        /// ↑描画処理ここまで
        ///

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
