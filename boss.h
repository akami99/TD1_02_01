#pragma once
#include <Novice.h>
#include "Data.h"

class Boss {
public:
	void BossMove(Boss_& boss, BossRengeAttak_& renge, ShortDistansAttak_& shortDist,
		Player_& player, Object& object, ShortDubleDistansAttak_& doubleShort, Shake& shake,
		Beam2& beam2, Projectile* projectiles, Shockwave* shockwaves, WarpAttak& warp);

	void SecondBossMove(Boss_& boss,  ShortDistansAttak_& shortDist, 
		Player_& player, Shake& shake,Beam2& beam2,  Shockwave* shockwaves, WarpAttak& warp, BossExprosive& explosive, Projectile* projectiles);

	//ボスの描画
	void BossDraw(Boss_ boss, Shake& shake);

	//=====================
	//範囲攻撃
	//=====================
	void BossRengeAttak(Boss_& boss, BossRengeAttak_& renge, Shake& shake);

	void DrawBossRengeAttak(BossRengeAttak_ renge);

	//======================
	//チャージ攻撃
	//======================

	void DrawBossChargeAttak(const Object& object);

	//=======================
	//ビーム攻撃
	//=======================
	void BeamAttack(Boss_& boss, Shake& shake);  // ビーム攻撃開始

	void DrawBeams(Boss_& boss);   // ビームを描画

	//イージング用の関数
	void BossMoveToPosition(Boss_& boss, const Vector2& targetPos, float easingSpeed);  // 目標位置にイージングで移動

	//========================================
	//近距離攻撃
	//========================================

	void ShortDistansAttak(Boss_& boss, ShortDistansAttak_& shortDist, Player_ player);

	void DrawShortDistansAttak(ShortDistansAttak_ shortDist);

	void DoubleShortDistansAttak(Boss_& boss, ShortDubleDistansAttak_& shortDobleDist, Player_ player);

	void DrawDoubleShortDistansAttak(ShortDubleDistansAttak_ shortDobleDist);

	//==========================
	//移動ビーム
	//==========================
	void Beam2Attak(Boss_& boss, Beam2& beam2, Shake& sheke);

	void Beam2HitBox(Beam2& beam2, Player_& player);

	void DrawBeam2(Beam2& beam2);


	void DrawWhole(Boss_& whole);

	//=============
	//第二形態の攻撃
	//=============

	static void UpdateProjectiles(Projectile* projectiles);

	// 全方位攻撃の初期化
	void InitializeAllRangeAttack(Boss_& beams);

	// 全方位攻撃の更新
	void UpdateAllRangeAttack(Boss_& beams);

	// 全方位攻撃の描画
	void DrawAllRangeAttack(Boss_& beams);

	//=====================
	//落下攻撃
	//=====================

	void BossFallAttak(Boss_& boss, Player_& player, Shake& shake, Shockwave* shockwaves, int maxShockwaves);

	//=====================
	//落下攻撃に出る衝撃波
	//=====================
	void UpdateShockwaves(Shockwave* shockwaves, int maxShockwaves, Player_& player);

	void DrawShockwaves(Shockwave* shockwaves, int maxShockwaves);

	//===================
	//ワープ攻撃
	//===================
	void BossWarpAttak(Boss_& boss, Player_& player, WarpAttak& warp, ShortDistansAttak_& shortDist);

	void DrawWarpAttak(WarpAttak& warp);

	//=====================================================================
	//当たり判定の作成(後でプレイヤーの向きによってバッグに当たり判定がないようにする)
	//=====================================================================

	//範囲攻撃
	void PlayerRengeHitBox(Player_& player, BossRengeAttak_& renge);

	//近距離攻撃
	void PlayerShortAttakHitBox(Player_& player, ShortDistansAttak_& shortAttak);

	//ビーム攻撃の当たり判定
	void PlayerBeamsHitBox(Player_& player, Boss_& boss);

	//連続攻撃の当たり判定
	void PlayerShortDobleAttakHitBox(Player_& player, ShortDubleDistansAttak_& doubleAttak);


	void AllRengeAttakHitBox(Boss_& allRange, Player_& player);

	void AllRengeAttakHitBox(Boss_& allRange,Player_& player);
	
	//=========================================
	//第2形態でボスのHPが75,50,25%になったらする攻撃
	//=========================================
	void BossExplosive(Boss_& boss, BossExprosive& explosive, Player_& player,Shake& shake);

	void DrawExplosive(BossExprosive& explosive);

	void BossFinishBro(Boss_& boss,Player_& player,Shake& shake);

	void DrawBossExplo(Boss_& boss);

private:

};

