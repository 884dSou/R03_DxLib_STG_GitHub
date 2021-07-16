//図形のソースファイル

//ヘッダーファイルの読み込み
#include"game.h"
#include"shape.h"
#include"math.h"

//グローバル変数

//関数

/// <summary>
/// 矩形領域同士の当たり判定をする関数
/// </summary>
/// <param name="a">領域A</param>
/// <param name="b">領域B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left<b.right&&
		a.top < b.bottom&&
		a.right > b.left&&
		a.bottom > b.top)
	{
		return TRUE;	//当たってる
	}
	return FALSE;		//当たってない
}


BOOL CheckCollPointToRect(iPOINT pt, RECT r)
{
	if (pt.x<r.right&&
		pt.y < r.bottom&&
		pt.x > r.left&&
		pt.y > r.top)
	{
		return TRUE;	//当たってる
	}
	return FALSE;		//当たってない
}

/// <summary>
/// 円と点の当たり判定
/// </summary>
/// <param name="pt">iPOINT型の点</param>
/// <param name="m">円領域</param>
/// <returns></returns>
BOOL CheckCollPointToMaru(iPOINT pt, MARU m)
{
	//三角関数を求めよう
	float a = pt.x - m.center.x;	//三角形の底辺の長さを求める
	float b = pt.y - m.center.y;	//三角形の高さの長さをもとんる
	float c = sqrtf(a * a + b * b);	//三角形の斜辺の長さを求める(sq rt = square root = 平方根)

	//斜辺の長さが、円の半径よりも短いならば、円の中にいる！
	if (c <= m.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 円と円の当たり判定
/// </summary>
/// <param name="maru1">円領域①</param>
/// <param name="maru2">円領域②</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
	//三角関数を求めよう
	float a = maru1.center.x - maru2.center.x;	//三角形の底辺の長さを求める
	float b = maru1.center.y - maru2.center.y;	//三角形の高さの長さをもとんる
	float c = sqrtf(a * a + b * b);				//三角形の斜辺の長さを求める(sq rt = square root = 平方根)

	//斜辺の長さが、円の半径①+円の半径②よりも短いならば、円の中にいる！
	if (c <= maru1.radius+maru2.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// RECT型を一時的に渡す
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns></returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//一時的にRECT型の変数を作って
	RECT rect = { left,top,right,bottom };

	//RECT型を返す
	return rect;
}

/// <summary>
/// RECTを利用して四角を描画
/// </summary>
/// <param name="r">RECT構造体</param>
/// <param name="color">描画する色</param>
/// <param name="b">中を塗りつぶさないなら「FALSE」／塗りつぶすなら「TRUE」</param>
void DrawRect(RECT r, unsigned int color, bool b)
{
	//因数をもとに描画
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// RECT型を利用して丸を描画
/// </summary>
/// <param name="c">MARUの構造体</param>
/// <param name="color">描画する色</param>
/// <param name="b">中を塗りつぶさないなら「FALSE」／塗りつぶすなら「TRUE」</param>
/// <param name="thick">線の太さ</param>
VOID DrawMaru(MARU c, unsigned int color, bool b, int thick = 1)
{
	//引数をもとに描画
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}