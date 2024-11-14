#pragma once
#include <Novice.h>
#include "Data.h"

class Boss {
public:
    void BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player);
    void BossDraw(Boss_ boss);
    void DrawBossRengeAttak(BossRengeAttak_ renge);

private:
};
