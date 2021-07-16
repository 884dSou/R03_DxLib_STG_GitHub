#pragma once
//マウス処理のヘッダ

//ヘッダファイル読込
#include "game.h"
#include "shape.h"

//マクロ定義
#define MOUSE_BUTON_CODE	129		//0x0000～0x0080まで

//構造体

//マウス構造体
struct MOUSE
{
	int InputValue = 0;					//GetMouseInputoの値が入る
	int WheelValue = 0;					//マウスホイールの回転量（奥：プラス／手前：マイナス）
	iPOINT Point;						//マウスの座標
	iPOINT OldPoint;					//マウスの直前の座標
	int OldButton[MOUSE_BUTON_CODE];	//直前のボタン入力
	int Button[MOUSE_BUTON_CODE];		//ボタン入力
};

//外部のグローバル変数

extern MOUSE mouse;	//マウス入力を取得

//プロトタイプ宣言

extern VOID MouseUpdate(VOID);
extern BOOL MouseDown(int MOUSE_INPUT_);
extern BOOL MouseUp(int MOUSE_INPUT_);
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime);
extern BOOL MouseClick(int MOUSE_INPUT);
extern VOID MouseDraw(VOID);

extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);
extern BOOL MouseMaruClick(MARU rect, int MOUSE_INPUT_);