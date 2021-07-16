//マウス処理のソースファイル

//ヘッダーファイルの読み込み
#include "game.h"
#include "mouse.h"
#include "FPS.h"

//グローバル変数

MOUSE mouse;	//マウス入力を取得

//関数

/// <summary>
/// マウスの入力情報の更新
/// </summary>
/// <param name=""></param>
VOID MouseUpdate(VOID)
{
	//マウスの以前の情報を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	mouse.OldButton[MOUSE_INPUT_LEFT] = mouse.Button[MOUSE_INPUT_LEFT];
	mouse.OldButton[MOUSE_INPUT_MIDDLE] = mouse.Button[MOUSE_INPUT_MIDDLE];
	mouse.OldButton[MOUSE_INPUT_RIGHT] = mouse.Button[MOUSE_INPUT_RIGHT];

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの座標が画面買いの場合は、画面内に収める
	if (mouse.Point.x < 0) { mouse.Point.x = 0; }
	if (mouse.Point.x > GAME_WIDTH) { mouse.Point.x = GAME_WIDTH; }
	if (mouse.Point.y < 0) { mouse.Point.y = 0; }
	if (mouse.Point.y > GAME_HEIGHT) { mouse.Point.y = GAME_HEIGHT; }

	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//左ボタンを押しているかチェック
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }	//押してる
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }	//押してない
	
	//中ボタンを押しているかチェック
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//押してる
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//押してない

	//右ボタンを押しているかチェック
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }	//押してる
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//押してない

	//ホイールの回転量を取得
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// ボタンを押しているか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns></returns>
BOOL MouseDown(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押しているとき
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//押してるとき
	}
	else
	{
		return FALSE;	//押してないとき
	}
}

/// <summary>
/// マウスを上げているか、マウスコード判断する
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns></returns>
BOOL MouseUp(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//直前は押していた
		&& mouse.Button[MOUSE_INPUT_] == 0	//今は押していない
		)
	{
		return TRUE;	//マウスをクリックした（押し続けても、１回のみ）
	}
	else
	{
		return FALSE;	//マウスをクリックしていないか、押し続けている
	}
}

/// <summary>
/// ボタンを押し上げたか、マウスコード判断する
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <param name="milliTime"></param>
/// <returns></returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime)
{
	//１秒は1000ミリ秒
	float MilliSec = 1000.0f;

	//押し続けている時間はミリ秒数×FPS値
	//例）1500ミリ秒押す/1000ミリ　→　1.5×60FPS = 90
	int UpdateTime = (milliTime / MilliSec) * fps.Value;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

/// <summary>
/// マウスをクリックしたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns>クリックしたら「TRUE」</returns>
BOOL MouseClick(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//直前は押していた
		&& mouse.Button[MOUSE_INPUT_] == 0	//今は押していない
		)
	{
		return TRUE;	//マウスをクリックした（押し続けても、１回のみ）
	}
	else
	{
		return FALSE;	//マウスをクリックしていないか、押し続けている
	}
}

/// <summary>
/// マウスの情報を描画する
/// </summary>
/// <param name=""></param>
VOID MouseDraw(VOID)
{
	if (Game_DEBUG == TRUE)
	{
		//マウスの座標を描画
		DrawFormatString(0, GAME_HEIGHT - 40, GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]", mouse.Point.x, mouse.Point.y);
	}

	return;
}

/// <summary>
/// マウスが矩形領域をクリックしたか？
/// </summary>
/// <param name="rect">矩形領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns></returns>
BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_)
{
	//点と資格の当たり判定
	if (CheckCollPointToRect(mouse.Point, rect) == TRUE)
	{
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}


BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_)
{
	//点と円の当たり判定
	if (CheckCollPointToMaru(mouse.Point, circle) == TRUE)
	{
		//マウスをクリックしているか
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}