#include "Main.h"
#include "Game.h"

#include "Title.h"	//	タイトル関係の関数を呼びたいので
#include "Stage.h"	//	ステージ関係の関数を呼びたいので
#include "Result.h"	//	リザルト（結果表示画面）関係の関数を呼びたいので
#include "Boble.h"	//	ボブル関係の関数を呼びたいので

int scene_num;		//	今どこのシーンにいるのかを見る用の変数
int scene_next;		//	次のシーン番号を入れる用の変数

extern int title_image;
extern int stage_image;
extern int title_back_image;


//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	最初はタイトルから開始
	scene_num = SCENE_TITLE;
	scene_next = SCENE_TITLE;

	//	タイトル背景の画像を読み込む
	title_image = LoadGraph("data/title.png");
	//	ステージ背景の画像を読み込む
	stage_image = LoadGraph("data/stage.png");
	title_back_image = LoadGraph("data/titleBack.png");
	
	//	タイトル画面の初期化
	TitleInit();
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	今のシーン番号と次に行きたいシーン番号が違っていたらシーンが切り替わるタイミング
	if( scene_num != scene_next ){
		//	今のシーン番号を新しく設定しなおして
		scene_num = scene_next;
		//	シーン番号に合わせた初期化処理を呼ぶ
		switch( scene_num ){
		case SCENE_TITLE:
			TitleInit();
			break;

		case SCENE_STAGE:
			StageInit();
			break;

		case SCENE_RESULT:
			
			break;
		}
	}

	//	今のシーン番号に合わせた更新処理を呼ぶ
	switch( scene_num ){
	case SCENE_TITLE:
		TitleUpdate();
		break;

	case SCENE_STAGE:
		StageUpdate();
		break;

	case SCENE_RESULT:
		
		break;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	今のシーン番号に合わせた描画処理を呼ぶ
	switch( scene_num ){
	case SCENE_TITLE:
		TitleRender();
		break;

	case SCENE_STAGE:
		StageRender();
		break;

	case SCENE_RESULT:
		
		break;
	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	TitleExit();
	StageExit();
	ResultExit();
	BobleExit();

	
}
