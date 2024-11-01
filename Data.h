#pragma once
#include<Vector2.h>
#include<Novice.h>

//nannka nihongode .h no komento utenaikara yorosiku nihongoni kaetoitene

//player
struct Player_ {
	Vector2 pos = { 32.0f,600.0f };
	Vector2 velocity = { 0.0f,0.0f };
	Vector2 acceleration = {0.0f,0.5f};
	float radius = 32;
	int isFlash=false;
	int isJanp=false;
	int janpNum =0;
	float speed = 8.0f;
	int isJanpSpeed = false;
};

//line(zimen)
struct Line {
	Vector2 pos = { 0.0f,600.0f };
};