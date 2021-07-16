//�}�`�̃\�[�X�t�@�C��

//�w�b�_�[�t�@�C���̓ǂݍ���
#include"game.h"
#include"shape.h"
#include"math.h"

//�O���[�o���ϐ�

//�֐�

/// <summary>
/// ��`�̈擯�m�̓����蔻�������֐�
/// </summary>
/// <param name="a">�̈�A</param>
/// <param name="b">�̈�B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left<b.right&&
		a.top < b.bottom&&
		a.right > b.left&&
		a.bottom > b.top)
	{
		return TRUE;	//�������Ă�
	}
	return FALSE;		//�������ĂȂ�
}


BOOL CheckCollPointToRect(iPOINT pt, RECT r)
{
	if (pt.x<r.right&&
		pt.y < r.bottom&&
		pt.x > r.left&&
		pt.y > r.top)
	{
		return TRUE;	//�������Ă�
	}
	return FALSE;		//�������ĂȂ�
}

/// <summary>
/// �~�Ɠ_�̓����蔻��
/// </summary>
/// <param name="pt">iPOINT�^�̓_</param>
/// <param name="m">�~�̈�</param>
/// <returns></returns>
BOOL CheckCollPointToMaru(iPOINT pt, MARU m)
{
	//�O�p�֐������߂悤
	float a = pt.x - m.center.x;	//�O�p�`�̒�ӂ̒��������߂�
	float b = pt.y - m.center.y;	//�O�p�`�̍����̒��������Ƃ��
	float c = sqrtf(a * a + b * b);	//�O�p�`�̎Εӂ̒��������߂�(sq rt = square root = ������)

	//�Εӂ̒������A�~�̔��a�����Z���Ȃ�΁A�~�̒��ɂ���I
	if (c <= m.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// �~�Ɖ~�̓����蔻��
/// </summary>
/// <param name="maru1">�~�̈�@</param>
/// <param name="maru2">�~�̈�A</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
	//�O�p�֐������߂悤
	float a = maru1.center.x - maru2.center.x;	//�O�p�`�̒�ӂ̒��������߂�
	float b = maru1.center.y - maru2.center.y;	//�O�p�`�̍����̒��������Ƃ��
	float c = sqrtf(a * a + b * b);				//�O�p�`�̎Εӂ̒��������߂�(sq rt = square root = ������)

	//�Εӂ̒������A�~�̔��a�@+�~�̔��a�A�����Z���Ȃ�΁A�~�̒��ɂ���I
	if (c <= maru1.radius+maru2.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// RECT�^���ꎞ�I�ɓn��
/// </summary>
/// <param name="left">��</param>
/// <param name="top">��</param>
/// <param name="right">�E</param>
/// <param name="bottom">��</param>
/// <returns></returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//�ꎞ�I��RECT�^�̕ϐ��������
	RECT rect = { left,top,right,bottom };

	//RECT�^��Ԃ�
	return rect;
}

/// <summary>
/// RECT�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="r">RECT�\����</param>
/// <param name="color">�`�悷��F</param>
/// <param name="b">����h��Ԃ��Ȃ��Ȃ�uFALSE�v�^�h��Ԃ��Ȃ�uTRUE�v</param>
void DrawRect(RECT r, unsigned int color, bool b)
{
	//���������Ƃɕ`��
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// RECT�^�𗘗p���Ċۂ�`��
/// </summary>
/// <param name="c">MARU�̍\����</param>
/// <param name="color">�`�悷��F</param>
/// <param name="b">����h��Ԃ��Ȃ��Ȃ�uFALSE�v�^�h��Ԃ��Ȃ�uTRUE�v</param>
/// <param name="thick">���̑���</param>
VOID DrawMaru(MARU c, unsigned int color, bool b, int thick = 1)
{
	//���������Ƃɕ`��
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}