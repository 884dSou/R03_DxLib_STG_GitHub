#pragma once

//�}�`�̃w�b�_�t�@�C��

//�w�b�_�[�t�@�C���̓ǂݍ���
#include"game.h"

//�}�N����`

//�\����

//int�^��POINT�\����
struct iPOINT
{
	int x = -1;
	int y = -1;
};

//�~�̍\����
struct MARU
{
	iPOINT center;	//���S�_
	int radius;	//���a
};

//�O���̃O���[�o���ϐ�

//�v���g�^�C�v�錾
extern BOOL CheckCollRectToRect(RECT a,RECT b);
extern BOOL CheckCollPointToRect(iPOINT pt,RECT p);
extern BOOL CheckCollPointToMaru(iPOINT pt,MARU m);
extern BOOL CheckCollMaruToMaru(MARU maru1,MARU maru2);

extern RECT GetRect(int left, int top, int right, int bottom);
extern void DrawRect(RECT r, unsigned int color, bool b);
extern VOID DrawMaru(MARU c, unsigned int color, bool b, int thick = 1);