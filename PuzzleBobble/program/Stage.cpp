#include "DxLib.h"
#include "Game.h"
#include "Stage.h"
#include "Float2.h"
#include "Hit.h"
#include "Boble.h"
#include <string>

StageBoble stage_boble[Stageboble];
bool stage_is_clear = false;

int shot_count = 0;
// Boble.cpp にある発射フラグを extern 宣言して参照する
extern bool is_shot;
bool prev_is_shot = false;

const int TURN_TO_DROP = 4;

void StageInit()
{
	stage_is_clear = false;
	shot_count = 0;
	prev_is_shot = false;

	BobleInit();

	int stage1_data[4][BOBLE_COLS] = {
		{ BOBLE_RED,   BOBLE_RED,   BOBLE_BLUE,  BOBLE_BLUE,  BOBLE_GREEN, BOBLE_GREEN, BOBLE_YELLOW, BOBLE_YELLOW },
		{ BOBLE_YELLOW,BOBLE_YELLOW,BOBLE_RED,   BOBLE_RED,   BOBLE_BLUE,  BOBLE_BLUE,  BOBLE_GREEN,  BOBLE_NONE   },
		{ BOBLE_NONE,  BOBLE_YELLOW,BOBLE_YELLOW,BOBLE_RED,   BOBLE_RED,   BOBLE_BLUE,  BOBLE_BLUE,   BOBLE_NONE   },
		{ BOBLE_NONE,  BOBLE_NONE,  BOBLE_GREEN, BOBLE_GREEN, BOBLE_YELLOW,BOBLE_YELLOW,BOBLE_NONE,   BOBLE_NONE   }
	};

	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < BOBLE_COLS; ++c) {
			boble_field[r][c] = stage1_data[r][c];
		}
	}
}

void DropCeiling()
{
	// 盤面データはそのままで、描画と判定の基準となる ceiling_offset を1段分下げる
	ceiling_offset += ROW_HEIGHT;
}

void StageUpdate()
{
	if (stage_is_clear) return;

	BobleUpdate();

	if (prev_is_shot && !is_shot) {
		shot_count++;
		if (shot_count >= TURN_TO_DROP) {
			DropCeiling();
			shot_count = 0;
		}
	}
	prev_is_shot = is_shot;

	bool has_bubble = false;
	for (int r = 0; r < BOBLE_ROWS; ++r) {
		for (int c = 0; c < BOBLE_COLS; ++c) {
			if (boble_field[r][c] != BOBLE_NONE) {
				has_bubble = true;
				break;
			}
		}
		if (has_bubble) break;
	}

	if (!has_bubble) {
		stage_is_clear = true;
	}
}

void StageRender()
{
	BobleRender();

	// 天井自体（押し降りてきている板）を描画する
	// BobleRender() の後に描画することで、めり込んでいるボールの上を隠して
	// 本来の「天井に押し出されている」感を出します
	if (ceiling_offset > 0.0f) {
		DrawBox(250, 0, 550, (int)ceiling_offset, GetColor(100, 100, 100), TRUE);
	}

	DrawLine(0, 370, 800, 370, GetColor(255, 255, 255), TRUE);
	DrawLine(250, 0, 250, 450, GetColor(255, 255, 255), TRUE);
	DrawLine(550, 0, 550, 450, GetColor(255, 255, 255), TRUE);
	DrawString(20, 20, "【ステージ画面】", GetColor(255, 255, 255));

	// NEXTボブル
	extern int next_next_shot_type; // Extern参照
	unsigned int next_color = GetColor(255, 0, 0);
	if (next_next_shot_type == BOBLE_BLUE)   next_color = GetColor(0, 0, 255);
	if (next_next_shot_type == BOBLE_GREEN)  next_color = GetColor(0, 255, 0);
	if (next_next_shot_type == BOBLE_YELLOW) next_color = GetColor(255, 255, 0);

	DrawString(150, 350, "NEXT", GetColor(255, 255, 255));
	DrawCircle(170, 390, (int)BOBLE_RADIUS, next_color, TRUE);

	// スコアとカウント
	std::string score_text = "SCORE: " + std::to_string(boble_score);
	DrawString(600, 20, score_text.c_str(), GetColor(255, 255, 255));

	std::string drops_text = "DROP IN: " + std::to_string(TURN_TO_DROP - shot_count);
	DrawString(600, 40, drops_text.c_str(), GetColor(255, 100, 100));

	if (stage_is_clear) {
		DrawString(350, 200, "STAGE CLEAR!", GetColor(255, 255, 0));
	}
}

void StageExit()
{
}