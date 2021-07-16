//�}�E�X�����̃\�[�X�t�@�C��

//�w�b�_�[�t�@�C���̓ǂݍ���
#include "game.h"
#include "mouse.h"
#include "FPS.h"

//�O���[�o���ϐ�

MOUSE mouse;	//�}�E�X���͂��擾

//�֐�

/// <summary>
/// �}�E�X�̓��͏��̍X�V
/// </summary>
/// <param name=""></param>
VOID MouseUpdate(VOID)
{
	//�}�E�X�̈ȑO�̏����擾
	mouse.OldPoint = mouse.Point;

	//�}�E�X�̈ȑO�̃{�^�����͂��擾
	mouse.OldButton[MOUSE_INPUT_LEFT] = mouse.Button[MOUSE_INPUT_LEFT];
	mouse.OldButton[MOUSE_INPUT_MIDDLE] = mouse.Button[MOUSE_INPUT_MIDDLE];
	mouse.OldButton[MOUSE_INPUT_RIGHT] = mouse.Button[MOUSE_INPUT_RIGHT];

	//�}�E�X�̍��W���擾
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//�}�E�X�̍��W����ʔ����̏ꍇ�́A��ʓ��Ɏ��߂�
	if (mouse.Point.x < 0) { mouse.Point.x = 0; }
	if (mouse.Point.x > GAME_WIDTH) { mouse.Point.x = GAME_WIDTH; }
	if (mouse.Point.y < 0) { mouse.Point.y = 0; }
	if (mouse.Point.y > GAME_HEIGHT) { mouse.Point.y = GAME_HEIGHT; }

	//�}�E�X�̉����Ă���{�^�����擾
	mouse.InputValue = GetMouseInput();

	//���{�^���������Ă��邩�`�F�b�N
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }	//�����Ă�
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }	//�����ĂȂ�
	
	//���{�^���������Ă��邩�`�F�b�N
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//�����Ă�
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//�����ĂȂ�

	//�E�{�^���������Ă��邩�`�F�b�N
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }	//�����Ă�
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//�����ĂȂ�

	//�z�C�[���̉�]�ʂ��擾
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// �{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns></returns>
BOOL MouseDown(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h�̃{�^���������Ă���Ƃ�
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//�����Ă�Ƃ�
	}
	else
	{
		return FALSE;	//�����ĂȂ��Ƃ�
	}
}

/// <summary>
/// �}�E�X���グ�Ă��邩�A�}�E�X�R�[�h���f����
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns></returns>
BOOL MouseUp(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.Button[MOUSE_INPUT_] == 0	//���͉����Ă��Ȃ�
		)
	{
		return TRUE;	//�}�E�X���N���b�N�����i���������Ă��A�P��̂݁j
	}
	else
	{
		return FALSE;	//�}�E�X���N���b�N���Ă��Ȃ����A���������Ă���
	}
}

/// <summary>
/// �{�^���������グ�����A�}�E�X�R�[�h���f����
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <param name="milliTime"></param>
/// <returns></returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime)
{
	//�P�b��1000�~���b
	float MilliSec = 1000.0f;

	//���������Ă��鎞�Ԃ̓~���b���~FPS�l
	//��j1500�~���b����/1000�~���@���@1.5�~60FPS = 90
	int UpdateTime = (milliTime / MilliSec) * fps.Value;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

/// <summary>
/// �}�E�X���N���b�N�������A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_"></param>
/// <returns>�N���b�N������uTRUE�v</returns>
BOOL MouseClick(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.Button[MOUSE_INPUT_] == 0	//���͉����Ă��Ȃ�
		)
	{
		return TRUE;	//�}�E�X���N���b�N�����i���������Ă��A�P��̂݁j
	}
	else
	{
		return FALSE;	//�}�E�X���N���b�N���Ă��Ȃ����A���������Ă���
	}
}

/// <summary>
/// �}�E�X�̏���`�悷��
/// </summary>
/// <param name=""></param>
VOID MouseDraw(VOID)
{
	if (Game_DEBUG == TRUE)
	{
		//�}�E�X�̍��W��`��
		DrawFormatString(0, GAME_HEIGHT - 40, GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]", mouse.Point.x, mouse.Point.y);
	}

	return;
}

/// <summary>
/// �}�E�X����`�̈���N���b�N�������H
/// </summary>
/// <param name="rect">��`�̈�</param>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns></returns>
BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_)
{
	//�_�Ǝ��i�̓����蔻��
	if (CheckCollPointToRect(mouse.Point, rect) == TRUE)
	{
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}


BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_)
{
	//�_�Ɖ~�̓����蔻��
	if (CheckCollPointToMaru(mouse.Point, circle) == TRUE)
	{
		//�}�E�X���N���b�N���Ă��邩
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}