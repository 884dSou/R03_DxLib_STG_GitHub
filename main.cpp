
//ヘッダファイルの読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include"keyboard.h"	//キーボードの処理
#include"FPS.h"			//FPSの処理

//マクロ定義
#define TAMA_DIV_MAX	4
#define TAMA_MAX	20

//構造体

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	BOOL IsDraw = FALSE;	//画像が描画できる？
};


//キャラクタの構造体
struct CHARCTOR
{
	IMAGE img;

	RECT coll;			//当たり判定の領域（四角）
	int speed =1;
	int scean = 0;		//当たったものによって変わる
};

//動画の構造体
struct MOVIE
{
	IMAGE img;

	int Volume = 255;	//ボリューム（最小）1～255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;
	char path[255];

	int Volume = -1;	//ボリューム（MIN 0～255 MAX）
	int playType = -1;
};

struct TAMA {
	int handle[TAMA_DIV_MAX];		//画像ハンドル
	char path[255];		//画像のパス

	int DivTate;		//分割数（縦）
	int DivYoko;		//分割数（横）
	int DivMax;			//分割総数

	int AnimeCnt = 0;		//アニメーションカウンタ
	int AnimeCntMAX = 0;	//アニメーションカウンタMAX

	int NowIndex = 0;		//現在の画像の要素数

	int x ;			//X位置
	int y ;			//Y位置
	int width ;		//高さ
	int height;		//幅

	int speed;

	RECT coll;		//当たり判定

	BOOL IsDraw = FALSE;		//描画できる？
};

//シーンを管理する変数
GAME_SCEAEN GameScene;				//現在のゲームシーン
GAME_SCEAEN OldGameScene;			//前回のゲームシーン
GAME_SCEAEN NextGameScene;			//次のゲームシーン

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;						//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードインのカウンタ
int fadeInCntMax = 0;				//フェードインのカウンタMAX

//弾の構造体
struct TAMA tama_moto;		//元
struct TAMA tama[TAMA_MAX];	//実際に使う数

int tamaShotCnt = 0;
int tamaShotCntMAX = 15;

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面（処理）
VOID ChangeDraw(VOID);	//切り替え画面（描画）

VOID ChangeScene(GAME_SCEAEN scene);	//シーン切り替え

VOID CollUpdatePlayer(CHARCTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARCTOR* chara);	//当たり判定

VOID TamaCollUpdate(TAMA* tama);	//弾の当たり判定

BOOL OnCollrect(RECT a, RECT b);	//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームデータの読み込み
VOID GameInit(VOID);

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//音楽の読み込み
BOOL LoadImg(IMAGE* img, const char* path);	//画像の読み込み

BOOL LoadImageDivMem(int* handle, const char* path, int divyoko, int divtate);	//画像を分割して読み込み

VOID DrawTama(TAMA* tama);		//弾の描画

//プログラムはWinMainから始まります
//Windowsのプログラミング方法＝（WinAPI）で動いている
//DxLibは、DirectXという、ゲーミングプログラミングを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)

{
	SetOutApplicationLogValidFlag(FALSE);					//log.txtを出力しない
	ChangeWindowMode(TRUE);									//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);							//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);		//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);					//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);						//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);							//アイコンファイルを読込
	SetWindowStyleMode(GAME_WINDOW_BAR);					//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);									//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);									//ウィンドウをずっとアクティブにする

	//DxLibの初期化
	if (DxLib_Init() == -1)			//DXライブラリの初期化処理
	{
		return -1;					//エラーが起きたら直ちに終了
	}

	

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);


	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化
	if (!GameLoad())
	{
		//ゲームのデータを読み込み
		DxLib_End();	//DxLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0) { break; }

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCEAEN_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCEAEN_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でない時
			if (GameScene != GAME_SCEAEN_CHANGE)
			{
				NextGameScene = GameScene;	//次のシーンを保存
				GameScene = GAME_SCEAEN_CHANGE;
			}
		}


		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();		//ダブルバッファリングした画面を描画

	}

	//読み込んだ画像を開放
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }

	//DxLib使用の終了処理
	DxLib_End();

	return 0;				//ソフトの終了
}

/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE/読み込めなかったらFALSE</returns>
BOOL GameLoad()
{
	//弾の分割数を設定
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//弾のパスをコピー
	strcpyDx(tama_moto.path, ".\\image\\Tama.png");

	//画像を分割して読み込み
	if (LoadImageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	//幅と高さを取得
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//位置を設定
	tama_moto.x = GAME_WIDTH / 2 + tama_moto.width / 2;
	tama_moto.y = GAME_HEIGHT / 2 + tama_moto.height / 2;

	tama_moto.speed = 10;		//速度

	//アニメを変える速度
	tama_moto.AnimeCntMAX = 15;

	//当たり判定の更新
	TamaCollUpdate(&tama_moto);

	//画像を表示しない
	tama_moto.IsDraw = FALSE;

	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	return TRUE;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	
}

//▼▼▼▼▼▼▼▼▼▼タイトル画面▼▼▼▼▼▼▼▼▼▼▼

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCEAEN scene)
{
		GameScene = scene;
		IsFadeIn = FALSE;
		IsFadeOut = TRUE;

		return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//処理
	TitleDraw();		//描画
	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

//▼▼▼▼▼▼▼▼▼▼▼プレイ画面▼▼▼▼▼▼▼▼▼▼▼
/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画
	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	if(KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		return;		//処理を強制終了
	}

	//スペースキーを押しているとき
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			//弾を発射する
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					//弾の発射
					tama[i].IsDraw = TRUE;

					//弾の位置
					tama[i].x = GAME_WIDTH / 2 - tama[i].width / 2;
					tama[i].y = GAME_HEIGHT / 2 - tama[i].height / 2;

					//弾の当たり判定の更新
					TamaCollUpdate(&tama[i]);

					break;

				}
			}
		}
		//弾の発射待ち
		if (tamaShotCntMAX < tamaShotCnt)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}


	//弾を飛ばす
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (tama[i].IsDraw == TRUE)
		{
			tama[i].y -= tama[i].speed;

			//画面買いに出たら描画しない
			if (
				tama[i].y + tama[i].height < 0 ||	//上
				tama[i].y > GAME_HEIGHT||			//下
				tama[i].x + tama[i].width < 0 ||	//左
				tama[i].x > GAME_WIDTH				//右
				)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{	
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されているとき
		if(tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);
		}
	}


	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

//▼▼▼▼▼▼▼▼▼▼エンド画面▼▼▼▼▼▼▼▼▼▼▼
/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画
	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{	
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

//▼▼▼▼▼▼▼▼▼▼切り替え画面▼▼▼▼▼▼▼▼▼▼
/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画
	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;		//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;		//カウンタを減らす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードアウト処理終了
		}
	}

	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトしていないとき
		GameScene = NextGameScene;		//次のシーンに切り替え
		OldGameScene = GameScene;		//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();			//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();				//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();				//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の領域更新（プレイヤー）
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARCTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width-55;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdate(CHARCTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 弾の当たり判定の領域更新
/// </summary>
/// <param name="chara">弾の当たり判定の領域</param>
VOID TamaCollUpdate(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// オブジェクトの衝突の確認
/// </summary>
/// <param name="a">当たり判定の領域（a）</param>
/// <param name="b">当たり判定の領域（b）</param>
/// <returns>当たったら「TRUE」当たらなかったら「FALSE」</returns>
BOOL OnCollrect(RECT a, RECT b)
{
	if (
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top
		)
	{
		//当たっているとき
		return TRUE;
	}
	else
	{
		//当たってないとき
		return FALSE;
	}
}

/// <summary>
/// 音楽の読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//音楽の読み込み
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"音楽読み込みエラー",
			MB_OK
		);

		return FALSE;
	}

	//その他設定
	audio->Volume = volume;
	audio->playType = playType;

	return TRUE;
}

/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="divyoko">分割するときの横の数</param>
/// <param name="divtate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(int *handle ,const char *path,int divyoko,int divtate)
{

	//弾の読み込み
	int IsTamaLoad = -1;	//画像が読み込めたか？

	//一時的に画像ハンドルを用意する
	int TamaHandle = LoadGraph(path);

	//読み込みエラー
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			path,
			"画像読み込みエラー",
			MB_OK
		);

		return FALSE;
	}

	//画像の幅と高さを取得
	int TamaWidth = -1;
	int TamaHeight = -1;
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//分割して読み込み
	IsTamaLoad = LoadDivGraph(
		path,					//画像のパス
		TAMA_DIV_MAX,			//分割総数
		divyoko, divtate,		//横の分割、縦の分割
		TamaWidth / divyoko, TamaHeight / divtate,	//画像１つ分の幅、高さ
		handle);				//連続で管理する配列の先頭アドレス

	//画像分割エラー
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			path,
			"画像分割エラー",
			MB_OK
		);

		return FALSE;
	}

	//一時的に読み込んだハンドルを開放
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="img">構造体のアドレス</param>
/// <param name="path">画像のパス</param>
/// <returns></returns>
BOOL LoadImg(IMAGE* img,const char* path)
{
	strcpyDx(img->path,path );
	img->handle = LoadGraph(img->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (img->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			img->path,			//メッセージタイトル
			"画像読み込みエラー！",	//メッセージ本文
			MB_OK					//ボタン
		);

		return FALSE;		//エラー終了
	}
	
	GetGraphSize(img->handle, &img->width, &img->height);

	return TRUE;
}


/// <summary>
/// 弾の描画
/// </summary>
/// <param name="tama">弾の構造体</param>
/// <returns></returns>
VOID DrawTama(TAMA* tama)
{
	//弾の描画ができるとき
	if (tama->IsDraw == TRUE)
	{
		//弾の描画
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//弾の添え字が弾の分割数の最大よりも小さいとき
			if (tama->NowIndex < tama->DivTate * tama->DivYoko - 1)
			{
				tama->NowIndex++;
			}
			else
			{
				tama->NowIndex = 0;
			}

			tama->AnimeCnt = 0;
		}
	}
}