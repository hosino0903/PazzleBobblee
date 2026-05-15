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
const int BOBLE_ROWS = 15; // 天井が下がることを加味して多めに確保しておくのがおすすめです
const int BOBLE_COLS = 8;

// ボールの半径
extern const float BOBLE_RADIUS;
extern const float ROW_HEIGHT; // 段の高さ

// 盤面データ
extern int boble_field[BOBLE_ROWS][BOBLE_COLS];

// スコア 
extern int boble_score;

// NEXT表示用
extern int next_next_shot_type;

// 天井の下がり具合（Y座標のオフセット）
extern float ceiling_offset;

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