#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX	256

//�L�[�{�[�h�\����

struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];		//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int OldAllKeyState[KEY_KIND_MAX];		//�ȑO�̃L�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���֐�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);		//���ׂẴL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KEY_INPUT_);		//����̃L�[�������Ă��邩
extern BOOL KeyUp(int KEY_INPUT_);			//����̃L�[�������Ă��邩
extern BOOL KeyClick(int KEY_INPUT_);		//����̃L�[���N���b�N������
extern BOOL KeyDownKeep(int KEY_INPUT_,int milSec);		//����̃L�[���w��̃~���b�����������Ă��邩�H