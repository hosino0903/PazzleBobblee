#pragma once

#define SCREEN_W	800
#define SCREEN_H	450

void GameInit();	//	初期化処理
void GameUpdate();	//	更新処理
void GameRender();	//	描画処理
void GameExit();	//	終了処理

//	各シーンの番号
enum {
	SCENE_TITLE,	//	０：タイトル
	SCENE_STAGE,	//	１：ステージ
	SCENE_RESULT,	//	２：リザルト（結果表示画面）
};

//	いろんなファイルで使う変数なので外部参照宣言しています
extern int scene_next;
