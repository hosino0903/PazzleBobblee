#pragma once

#define Stageboble 20

struct StageBoble
{
	float x;
	float y;
};
//	ステージで使う関数の宣言
void StageInit();
void StageUpdate();
void StageRender();
void StageExit();
