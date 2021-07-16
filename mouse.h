#pragma once
//�}�E�X�����̃w�b�_

//�w�b�_�t�@�C���Ǎ�
#include "game.h"
#include "shape.h"

//�}�N����`
#define MOUSE_BUTON_CODE	129		//0x0000�`0x0080�܂�

//�\����

//�}�E�X�\����
struct MOUSE
{
	int InputValue = 0;					//GetMouseInputo�̒l������
	int WheelValue = 0;					//�}�E�X�z�C�[���̉�]�ʁi���F�v���X�^��O�F�}�C�i�X�j
	iPOINT Point;						//�}�E�X�̍��W
	iPOINT OldPoint;					//�}�E�X�̒��O�̍��W
	int OldButton[MOUSE_BUTON_CODE];	//���O�̃{�^������
	int Button[MOUSE_BUTON_CODE];		//�{�^������
};

//�O���̃O���[�o���ϐ�

extern MOUSE mouse;	//�}�E�X���͂��擾

//�v���g�^�C�v�錾

extern VOID MouseUpdate(VOID);
extern BOOL MouseDown(int MOUSE_INPUT_);
extern BOOL MouseUp(int MOUSE_INPUT_);
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime);
extern BOOL MouseClick(int MOUSE_INPUT);
extern VOID MouseDraw(VOID);

extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);
extern BOOL MouseMaruClick(MARU rect, int MOUSE_INPUT_);