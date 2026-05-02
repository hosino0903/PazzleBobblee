#include "Main.h"
#include "Game.h"
#include "Stage.h"
#include "Float2.h"
#include "Hit.h"

//=================================================================================
//	ステージの処理
//=================================================================================

//	プレイヤーの移動スピード
#define PLAYER_SPEED	5.0f
//	プレイヤーやＮＰＣのサイズ
#define CHARA_SIZE		50.0f

//	ステージで使う画像データ変数の外部参照宣言
extern int stage_image;		//	ステージ背景画像
extern int player_image;	//	プレイヤー画像
extern int npc_image;		//	ＮＰＣ画像

Float2 player_pos;	//	プレイヤー座標
Float2 npc_pos;		//	ＮＰＣ座標
Float2 chara_size;	//	当たり判定で使うサイズ

//---------------------------------------------------------------------------------
//	初期化
//---------------------------------------------------------------------------------
void StageInit()
{
	//	プレイヤー変数の初期設定
	player_pos.x = 100.0f;
	player_pos.y = 200.0f;
	//	ＮＰＣ
	npc_pos.x = 500.0f;
	npc_pos.y = 200.0f;
	//	プレイヤーやＮＰＣの大きさ
	chara_size.x = CHARA_SIZE;
	chara_size.y = CHARA_SIZE;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void StageUpdate()
{
	//	十字キーでプレイヤーの座標の更新
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_pos.x += PLAYER_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_pos.x -= PLAYER_SPEED;
	if( CheckHitKey( KEY_INPUT_DOWN ) )		player_pos.y += PLAYER_SPEED;
	if( CheckHitKey( KEY_INPUT_UP ) )		player_pos.y -= PLAYER_SPEED;

	//	プレイヤーとＮＰＣの当たり判定（四角同士の当たり判定：Float2 バージョン）
	if( CheckBoxHit( player_pos, chara_size, npc_pos, chara_size ) ){
		//	当たったらリザルト（結果表示）画面へ
		scene_next = SCENE_RESULT;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void StageRender()
{
	//	ステージ背景の描画
	DrawGraph( 0, 0, stage_image, TRUE );

	//	ＮＰＣ画像の描画
	DrawGraphF( npc_pos.x, npc_pos.y, npc_image, TRUE );
	//	プレイヤーの描画
	DrawGraphF( player_pos.x, player_pos.y, player_image, TRUE );

	DrawString( 20, 20, "【ステージ画面】", GetColor( 255, 255, 255 ) );
	DrawString( 20, 40, "十字キー：プレイヤー移動", GetColor( 255, 255, 255 ) );
	DrawString( 20, 60, "プレイヤーとＮＰＣが当たったら結果表示画面へ", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void StageExit()
{
}
