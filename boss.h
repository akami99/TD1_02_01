#pragma once
#include <Novice.h>
#include "Data.h"

class Boss {
public:
	void BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist, Player_& player, Object& object,ShortDubleDistansAttak_& doubleShort);

	void BossDraw(Boss_ boss);

	void DrawBossRengeAttak(BossRengeAttak_ renge);

	void DrawBossChargeAttak(const Object& object);

	void BeamAttack(Boss_& boss);  // ビーム攻撃開始

	void DrawBeams(Boss_& boss);   // ビームを描画

	void BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed);  // 目標位置にイージングで移動

	void DrawShortDistansAttak(ShortDistansAttak_ shortDist);

	void DrawDoubleShortDistansAttak(ShortDubleDistansAttak_ shortDobleDist);

private:

};

