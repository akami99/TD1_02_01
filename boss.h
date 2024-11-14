#pragma once
#include <Novice.h>
#include "Data.h"

class Boss {
public:
    // ボスの動き
    void BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player);

    // ボスを描画する
    void BossDraw(Boss_ boss);

    // 範囲攻撃を描画する
    void DrawBossRengeAttak(BossRengeAttak_ renge);

    // 近距離攻撃を描画する
    void DrawShortDistansAttak(ShortDistansAttak_ shortDist);

private:
    // メンバ変数はここに追加する（必要なら）
};
