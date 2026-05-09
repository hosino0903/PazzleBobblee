#include"Boble.h"
#include"Main.h"
#include"Float2.h"


Boble boble;
// 発射中のボール


// 発射中のボール
Float2 shot_pos;
Float2 shot_vel;
int shot_type = BOBLE_NONE;
bool is_shot = false;
int next_shot_type = BOBLE_RED; // 待機中のボブルの種類

void BobleInit()
{
	

}

void BobleUpdate()
{
    // 発射していないときにスペースで発射
    if (!is_shot && CheckHitKey(KEY_INPUT_SPACE)) {
        shot_pos.x = 400;
        shot_pos.y = 400;
        shot_vel.x = 0;
        shot_vel.y = -8;
        shot_type = next_shot_type;
        is_shot = true;
        next_shot_type = 1 + GetRand(BOBLE_MAX - 2);
    }

    // 発射中のボブルを移動
    if (is_shot) {
        shot_pos.x += shot_vel.x;
        shot_pos.y += shot_vel.y;

        // 画面外に出たら発射終了
        if (shot_pos.y < 0) {
            is_shot = false;
        }
    }

}

void BobleRender()
{
    // 発射中のボブルを描画
    if (is_shot) {
        unsigned int color = GetColor(255, 0, 0);
        if (shot_type == BOBLE_BLUE) color = GetColor(0, 0, 255);
        if (shot_type == BOBLE_GREEN) color = GetColor(0, 255, 0);
        if (shot_type == BOBLE_YELLOW) color = GetColor(255, 255, 0);

        DrawCircle((int)shot_pos.x, (int)shot_pos.y, 18, color, TRUE);
    }

    // 待機中のボブルを描画
    if (!is_shot) {
        unsigned int color = GetColor(255, 0, 0);
        if (next_shot_type == BOBLE_BLUE) color = GetColor(0, 0, 255);
        if (next_shot_type == BOBLE_GREEN) color = GetColor(0, 255, 0);
        if (next_shot_type == BOBLE_YELLOW) color = GetColor(255, 255, 0);

        int px = 400;
        int py = 400;
        DrawCircle(px, py, 18, color, TRUE);
    }
}

void BobleExit()
{


}