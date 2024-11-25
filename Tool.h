#pragma once

// 2点間のベクトルを求める関数
float Measurement(float pos1, float pos2);

// ベクトルの長さを求める関数
float Length(float vectorX, float vectorY);

// 外積を求める関数(1から見て2がどちら側か-で左側)
float CrossProduct(float x1, float y1, float x2, float y2);