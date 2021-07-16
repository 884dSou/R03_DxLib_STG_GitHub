#pragma once

//図形のヘッダファイル

//ヘッダーファイルの読み込み
#include"game.h"

//マクロ定義

//構造体

//int型のPOINT構造体
struct iPOINT
{
	int x = -1;
	int y = -1;
};

//円の構造体
struct MARU
{
	iPOINT center;	//中心点
	int radius;	//半径
};

//外部のグローバル変数

//プロトタイプ宣言
extern BOOL CheckCollRectToRect(RECT a,RECT b);
extern BOOL CheckCollPointToRect(iPOINT pt,RECT p);
extern BOOL CheckCollPointToMaru(iPOINT pt,MARU m);
extern BOOL CheckCollMaruToMaru(MARU maru1,MARU maru2);

extern RECT GetRect(int left, int top, int right, int bottom);
extern void DrawRect(RECT r, unsigned int color, bool b);
extern VOID DrawMaru(MARU c, unsigned int color, bool b, int thick = 1);