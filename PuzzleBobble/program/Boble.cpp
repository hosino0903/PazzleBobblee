#include"Boble.h"
#include"Main.h"
#include"Float2.h"
#include <math.h>
#include <vector>
#include <string>

Boble boble;

// 盤面データ (ヘッダでの extern に対応する実体)
int boble_field[BOBLE_ROWS][BOBLE_COLS];

// スコア (ヘッダでの extern に対応する実体)
int boble_score = 0;

// 発射中のボール
Float2 shot_pos;
Float2 shot_vel;
int shot_type = BOBLE_NONE;
bool is_shot = false;

// 待機中のボブルの種類
int next_shot_type = BOBLE_RED;

// 次の次に待機中のボブル（NEXT表示用）の種類の実体 (extern はいらない)
int next_next_shot_type = BOBLE_RED;

// 天井のオフセット（実体）
float ceiling_offset = 0.0f;

// 発射角度 (ラジアン) 初期値は真上(90度 = PI/2)
float shot_angle = 3.14159265f / 2.0f;

// 待機タイマー (60FPS想定: 60 * 3秒 = 180フレーム)
int start_wait_timer = 180;

// ゲームオーバー状態のフラグ
bool is_gameover = false;

// ボールの半径と直径（ヘッダのexternに対応させるためconst floatの実体定義。こちらもexternは付けない）
const float BOBLE_RADIUS = 18.0f;
const float BOBLE_DIAMETER = 36.0f;
// 六角形配置のための段差Y幅 (直径 * sin(60度))
const float ROW_HEIGHT = BOBLE_DIAMETER * 0.8660254f;

// ゲームオーバー判定ライン（発射台の少し上）
const float DEAD_LINE_Y = 350.0f;

void BobleInit()
{
    start_wait_timer = 180; // 初期化時にタイマーをセット
    is_shot = false;
    shot_angle = 3.14159265f / 2.0f; // 角度も初期化
    boble_score = 0; // スコアの初期化
    ceiling_offset = 0.0f; // 天井を初期位置に戻す
    is_gameover = false;   // ゲームオーバーフラグをリセット

    // ステージ開始時の最初の待機ボールもランダムにする
    next_shot_type = 1 + GetRand(BOBLE_MAX - 2);
    next_next_shot_type = 1 + GetRand(BOBLE_MAX - 2);

    // 盤面データを一旦空(BOBLE_NONE)で初期化
    for (int r = 0; r < BOBLE_ROWS; ++r) {
        for (int c = 0; c < BOBLE_COLS; ++c) {
            boble_field[r][c] = BOBLE_NONE;
        }
    }
}

// 座標から盤面の行列のインデックスを計算するヘルパー関数
void GetGridPos(float x, float y, int& out_r, int& out_c)
{
    // 天井のオフセットを引いてから行を計算する
    out_r = (int)((y - ceiling_offset - BOBLE_RADIUS) / ROW_HEIGHT + 0.5f);

    if (out_r < 0) out_r = 0;
    if (out_r >= BOBLE_ROWS) out_r = BOBLE_ROWS - 1;

    // 行が奇数なら半マスズレるのを考慮して列を計算
    float offset_x = (out_r % 2 == 1) ? BOBLE_RADIUS : 0.0f;
    out_c = (int)((x - 250.0f - offset_x) / BOBLE_DIAMETER);

    if (out_c < 0) out_c = 0;
    if (out_c >= BOBLE_COLS) out_c = BOBLE_COLS - 1;
}

// 配列インデックスから描画中心座標を取得する関数
void GetScreenPos(int r, int c, float& out_x, float& out_y)
{
    float offset_x = (r % 2 == 1) ? BOBLE_RADIUS : 0.0f;
    out_x = 250.0f + offset_x + (c * BOBLE_DIAMETER) + BOBLE_RADIUS;
    out_y = ceiling_offset + BOBLE_RADIUS + (r * ROW_HEIGHT); // 天井のオフセットを足す
}

// 周囲6方向の座標オフセット（行が偶数か奇数かで異なる）
void GetNeighbors(int r, int c, std::vector<std::pair<int, int>>& neighbors)
{
    // === 省略せず中身は既存のまま ===
    neighbors.clear();
    int odd_offset[6][2] = {
        {-1, 0}, {-1, 1},
        { 0,-1}, { 0, 1},
        { 1, 0}, { 1, 1}
    };
    int even_offset[6][2] = {
        {-1,-1}, {-1, 0},
        { 0,-1}, { 0, 1},
        { 1,-1}, { 1, 0}
    };

    auto& offset = (r % 2 == 1) ? odd_offset : even_offset;

    for (int i = 0; i < 6; ++i) {
        int nr = r + offset[i][0];
        int nc = c + offset[i][1];
        if (nr >= 0 && nr < BOBLE_ROWS && nc >= 0 && nc < BOBLE_COLS) {
            if (nr % 2 == 1 && nc == BOBLE_COLS - 1) continue;
            neighbors.push_back({ nr, nc });
        }
    }
}

// 空中に浮いてしまった（天井と繋がっていない）ボールを探して消す処理
void DropFloatingBubbles()
{
    // === 既存のまま ===
    bool connected[BOBLE_ROWS][BOBLE_COLS] = { false };
    std::vector<std::pair<int, int>> stack;

    for (int c = 0; c < BOBLE_COLS; ++c) {
        if (boble_field[0][c] != BOBLE_NONE) {
            connected[0][c] = true;
            stack.push_back({ 0, c });
        }
    }

    while (!stack.empty()) {
        auto pos = stack.back();
        stack.pop_back();

        int pr = pos.first;
        int pc = pos.second;

        std::vector<std::pair<int, int>> neighbors;
        GetNeighbors(pr, pc, neighbors);

        for (auto& n : neighbors) {
            int nr = n.first;
            int nc = n.second;
            if (!connected[nr][nc] && boble_field[nr][nc] != BOBLE_NONE) {
                connected[nr][nc] = true;
                stack.push_back({ nr, nc });
            }
        }
    }

    for (int r = 0; r < BOBLE_ROWS; ++r) {
        for (int c = 0; c < BOBLE_COLS; ++c) {
            if (boble_field[r][c] != BOBLE_NONE && !connected[r][c]) {
                boble_field[r][c] = BOBLE_NONE;
                boble_score += 20;
            }
        }
    }
}

// 繋がったボールを判定し、3つ以上なら消去し、浮いたボールも落とす処理
void CheckMatchAndDrop(int start_r, int start_c, int type)
{
    // === 既存のまま ===
    std::vector<std::pair<int, int>> matched;
    bool checked[BOBLE_ROWS][BOBLE_COLS] = { false };
    std::vector<std::pair<int, int>> stack;

    stack.push_back({ start_r, start_c });
    checked[start_r][start_c] = true;

    while (!stack.empty()) {
        auto pos = stack.back();
        stack.pop_back();
        matched.push_back(pos);

        int pr = pos.first;
        int pc = pos.second;

        std::vector<std::pair<int, int>> neighbors;
        GetNeighbors(pr, pc, neighbors);

        for (auto& n : neighbors) {
            int nr = n.first;
            int nc = n.second;
            if (!checked[nr][nc] && boble_field[nr][nc] == type) {
                checked[nr][nc] = true;
                stack.push_back({ nr, nc });
            }
        }
    }

    if (matched.size() >= 3) {
        for (auto& p : matched) {
            boble_field[p.first][p.second] = BOBLE_NONE;
            boble_score += 10;
        }
        DropFloatingBubbles();
    }
}

void BobleUpdate()
{
    if (is_gameover) return; // ゲームオーバー時は入力を受け付けない

    if (start_wait_timer > 0) {
        start_wait_timer--;
    }

    if (!is_shot && start_wait_timer <= 0) {
        if (CheckHitKey(KEY_INPUT_LEFT)) {
            shot_angle += 0.05f;
            if (shot_angle > 3.14159265f) shot_angle = 3.14159265f;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT)) {
            shot_angle -= 0.05f;
            if (shot_angle < 0.0f) shot_angle = 0.0f;
        }

        if (CheckHitKey(KEY_INPUT_SPACE)) {
            shot_pos.x = 400;
            shot_pos.y = 370;

            float speed = 8.0f;
            shot_vel.x = cosf(shot_angle) * speed;
            shot_vel.y = -sinf(shot_angle) * speed;

            shot_type = next_shot_type;
            next_shot_type = next_next_shot_type;
            next_next_shot_type = 1 + GetRand(BOBLE_MAX - 2);

            is_shot = true;
        }
    }

    if (is_shot) {
        shot_pos.x += shot_vel.x;
        shot_pos.y += shot_vel.y;

        // 白いライン(X=250 と X=550)で反射
        if (shot_pos.x - BOBLE_RADIUS < 250.0f) {
            shot_pos.x = 250.0f + BOBLE_RADIUS;
            shot_vel.x = -shot_vel.x;
        }
        else if (shot_pos.x + BOBLE_RADIUS > 550.0f) {
            shot_pos.x = 550.0f - BOBLE_RADIUS;
            shot_vel.x = -shot_vel.x;
        }

        bool attached = false;

        // 天井判定 (ceiling_offsetを加味する)
        if (shot_pos.y <= ceiling_offset + BOBLE_RADIUS) {
            attached = true;
        }
        else {
            // 他の盤面上のボールとの当たり判定
            for (int r = 0; r < BOBLE_ROWS; ++r) {
                for (int c = 0; c < BOBLE_COLS; ++c) {
                    if (boble_field[r][c] != BOBLE_NONE) {
                        float target_x, target_y;
                        GetScreenPos(r, c, target_x, target_y);

                        float dx = shot_pos.x - target_x;
                        float dy = shot_pos.y - target_y;
                        float distance = sqrtf(dx * dx + dy * dy);

                        // 接触判定
                        if (distance <= BOBLE_DIAMETER) {
                            attached = true;
                            break;
                        }
                    }
                }
                if (attached) break;
            }
        }

        // 天井や他のボールにぶつかった場合の盤面登録処理
        if (attached) {
            int r, c;
            GetGridPos(shot_pos.x, shot_pos.y, r, c);

            while (r < BOBLE_ROWS && boble_field[r][c] != BOBLE_NONE) {
                r++;
                float offset_x = (r % 2 == 1) ? BOBLE_RADIUS : 0.0f;
                c = (int)((shot_pos.x - 250.0f - offset_x) / BOBLE_DIAMETER);
                if (c < 0) c = 0;
                if (c >= BOBLE_COLS) c = BOBLE_COLS - 1;
            }

            if (r % 2 == 1 && c >= BOBLE_COLS - 1) c = BOBLE_COLS - 2;

            if (r < BOBLE_ROWS) {
                boble_field[r][c] = shot_type;
                CheckMatchAndDrop(r, c, shot_type);

                // ゲームオーバー判定
                for (int gr = 0; gr < BOBLE_ROWS; ++gr) {
                    for (int gc = 0; gc < BOBLE_COLS; ++gc) {
                        if (boble_field[gr][gc] != BOBLE_NONE) {
                            float bx, by;
                            GetScreenPos(gr, gc, bx, by);
                            // ボールの下端がデッドラインを超えたらゲームオーバー
                            if (by + BOBLE_RADIUS >= DEAD_LINE_Y) {
                                is_gameover = true;
                            }
                        }
                    }
                }
            }
            else {
                // 範囲外まで行ってしまった場合も念のためゲームオーバー
                is_gameover = true;
            }

            is_shot = false;
        }
    }
}

void BobleRender()
{
    // 盤面に張り付いた（固定された）ボブルの描画
    for (int r = 0; r < BOBLE_ROWS; ++r) {
        for (int c = 0; c < BOBLE_COLS; ++c) {
            if (boble_field[r][c] != BOBLE_NONE) {
                unsigned int color = GetColor(255, 0, 0);
                if (boble_field[r][c] == BOBLE_BLUE) color = GetColor(0, 0, 255);
                if (boble_field[r][c] == BOBLE_GREEN) color = GetColor(0, 255, 0);
                if (boble_field[r][c] == BOBLE_YELLOW) color = GetColor(255, 255, 0);

                float draw_x, draw_y;
                GetScreenPos(r, c, draw_x, draw_y);
                DrawCircle((int)draw_x, (int)draw_y, (int)BOBLE_RADIUS, color, TRUE);
            }
        }
    }

    // デッドライン（ゲームオーバーのライン）をうっすら描画（見栄えと確認用）
    DrawLine(250, (int)DEAD_LINE_Y, 550, (int)DEAD_LINE_Y, GetColor(100, 100, 100));

    // 発射中のボブルを描画
    if (is_shot) {
        unsigned int color = GetColor(255, 0, 0);
        if (shot_type == BOBLE_BLUE) color = GetColor(0, 0, 255);
        if (shot_type == BOBLE_GREEN) color = GetColor(0, 255, 0);
        if (shot_type == BOBLE_YELLOW) color = GetColor(255, 255, 0);

        DrawCircle((int)shot_pos.x, (int)shot_pos.y, (int)BOBLE_RADIUS, color, TRUE);
    }

    // 待機中のボブルと発射角度ガイドを描画
    if (!is_shot) {
        unsigned int color = GetColor(255, 0, 0);
        if (next_shot_type == BOBLE_BLUE) color = GetColor(0, 0, 255);
        if (next_shot_type == BOBLE_GREEN) color = GetColor(0, 255, 0);
        if (next_shot_type == BOBLE_YELLOW) color = GetColor(255, 255, 0);

        // 発射台の位置(400, 370)に待機中のボブルを描画
        DrawCircle(400, 370, (int)BOBLE_RADIUS, color, TRUE);

        // 待機時間が終了し、ゲームオーバーでなければ発射角度（ガイド）を描画
        if (start_wait_timer <= 0 && !is_gameover) {
            int guide_length = 50;
            int end_x = 400 + (int)(cosf(shot_angle) * guide_length);
            int end_y = 370 - (int)(sinf(shot_angle) * guide_length);
            DrawLine(400, 370, end_x, end_y, GetColor(255, 255, 255));
        }
    }

    // ゲームオーバーの文字表示
    if (is_gameover) {
        // 必要に応じてフォントや位置を調整してください
        DrawString(360, 240, "GAME OVER", GetColor(255, 0, 0));
    }
}

void BobleExit()
{
}