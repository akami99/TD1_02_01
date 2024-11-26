#include "GameInitializer.h"

// 初期化関数の実装
void InitializeGame(Player_& player, Flash_& flash, Boss_& boss, Line& line) {
    // プレイヤーの初期化
    player.pos = { 32.0f, 600.0f };
    player.direction = { 1.0f, 0.0f };
    player.velocity = { 0.0f, 0.0f };
    player.hp = 300;
    player.energy = player.kMaxEnergy;
    player.isFlash = false;
    player.isHighFlash = false;
    player.isJanp = false;
    player.isRight = true;
    player.isLeft = false;

    // フラッシュライトの初期化
    flash.pos = { 0.0f, 0.0f };
    flash.direction = { 0.0f, 0.0f };
    flash.range = 192.0f;
    flash.highRange = 288.0f;

    // ボスの初期化
    boss.pos = { 1000.0f, 472.0f };
    boss.hp = 200;
    boss.secondHp = 200;
    boss.attakNo = 0;
    boss.attakStandTime = 120;
    boss.isAttak = false;
    boss.vanishTime = 0;
    boss.isFloating = false;
    boss.attackCount = 0;

    // ライン（地面と背景）の初期化
    line.pos = { 0.0f, 600.0f };
    line.endPos = { 1280.0f, 600.0f };
}