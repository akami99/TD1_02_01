#pragma once
#include <Novice.h>
#include "Data.h"

class Boss {
public:
	void BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist,
		Player_& player, Object& object, ShortDubleDistansAttak_& doubleShort, Shake& shake,
		Beam2& beam2,Projectile* projectiles, int& timer);

	void BossDraw(Boss_ boss,Shake& shake);

	void DrawBossRengeAttak(BossRengeAttak_ renge);

	void DrawBossChargeAttak(const Object& object);

	void BeamAttack(Boss_& boss,Shake& shake);  // ビーム攻撃開始

	void DrawBeams(Boss_& boss);   // ビームを描画

	void BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed);  // 目標位置にイージングで移動

	void DrawShortDistansAttak(ShortDistansAttak_ shortDist);

	void DrawDoubleShortDistansAttak(ShortDubleDistansAttak_ shortDobleDist);

	void DrawBeam2(Beam2& beam2);

	//第二形態の攻撃

	static void UpdateProjectiles(Projectile* projectiles);
	
private:

};



 