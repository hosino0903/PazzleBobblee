#pragma once


// ボールの種類
enum BobleType {
    BOBLE_NONE = 0,
    BOBLE_RED,
    BOBLE_BLUE,
    BOBLE_GREEN,
    BOBLE_YELLOW,
    BOBLE_MAX
};
// 盤面サイズ
const int BOBLE_ROWS = 10;
const int BOBLE_COLS = 8;

// 盤面データ
extern int boble_field[BOBLE_ROWS][BOBLE_COLS];
struct Boble
{
	float x;
	float y;
	float speed;
};

void BobleInit();
void BobleUpdate();
void BobleRender();
void BobleExit();