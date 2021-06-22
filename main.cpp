
//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include"keyboard.h"	//�L�[�{�[�h�̏���
#include"FPS.h"			//FPS�̏���

//�\����

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	BOOL IsDraw = FALSE;	//�摜���`��ł���H
};


//�L�����N�^�̍\����
struct CHARCTOR
{
	IMAGE img;

	RECT coll;			//�����蔻��̗̈�i�l�p�j
	int speed =1;
	int scean = 0;		//�����������̂ɂ���ĕς��
};

//����̍\����
struct MOVIE
{
	IMAGE img;

	int Volume = 255;	//�{�����[���i�ŏ��j1�`255�i�ő�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;
	char path[255];

	int Volume = -1;	//�{�����[���iMIN 0�`255 MAX�j
	int playType = -1;
};

//�V�[�����Ǘ�����ϐ�
GAME_SCEAEN GameScene;				//���݂̃Q�[���V�[��
GAME_SCEAEN OldGameScene;			//�O��̃Q�[���V�[��
GAME_SCEAEN NextGameScene;			//���̃Q�[���V�[��

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;						//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = 0;				//�t�F�[�h�C���̃J�E���^MAX

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCEAEN scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARCTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARCTOR* chara);	//�����蔻��

BOOL OnCollrect(RECT a, RECT b);	//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̓ǂݍ���
VOID GameInit(VOID);

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//���y�̓ǂݍ���
BOOL LoadImg(IMAGE* img, const char* path);	//�摜�̓ǂݍ���

//�v���O������WinMain����n�܂�܂�
//Windows�̃v���O���~���O���@���iWinAPI�j�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[�~���O�v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)

{
	SetOutApplicationLogValidFlag(FALSE);					//log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);									//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);							//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);		//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);					//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);						//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);							//�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);					//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);									//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);									//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//DxLib�̏�����
	if (DxLib_Init() == -1)			//DX���C�u�����̏���������
	{
		return -1;					//�G���[���N�����璼���ɏI��
	}

	

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����
	if (!GameLoad())
	{
		//�Q�[���̃f�[�^��ǂݍ���
		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCEAEN_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCEAEN_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ���
			if (GameScene != GAME_SCEAEN_CHANGE)
			{
				NextGameScene = GameScene;	//���̃V�[����ۑ�
				GameScene = GAME_SCEAEN_CHANGE;
			}
		}


		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();		//�_�u���o�b�t�@�����O������ʂ�`��

	}

	//DxLib�g�p�̏I������
	DxLib_End();

	return 0;				//�\�t�g�̏I��
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE/�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad()
{
	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	
}

//���������������������^�C�g����ʁ���������������������

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCEAEN scene)
{
		GameScene = scene;
		IsFadeIn = FALSE;
		IsFadeOut = TRUE;

		return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//����
	TitleDraw();		//�`��
	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}
	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

//�����������������������v���C��ʁ���������������������
/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��
	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	if(KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;		//�����������I��
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{	
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

//���������������������G���h��ʁ���������������������
/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��
	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{	
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

//���������������������؂�ւ���ʁ�������������������
/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��
	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;		//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;		//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�A�E�g�����I��
		}
	}

	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g���Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;		//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;		//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();			//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();				//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();				//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�v���C���[�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARCTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width-55;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARCTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �I�u�W�F�N�g�̏Փ˂̊m�F
/// </summary>
/// <param name="a">�����蔻��̗̈�ia�j</param>
/// <param name="b">�����蔻��̗̈�ib�j</param>
/// <returns>����������uTRUE�v������Ȃ�������uFALSE�v</returns>
BOOL OnCollrect(RECT a, RECT b)
{
	if (
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top
		)
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������ĂȂ��Ƃ�
		return FALSE;
	}
}

/// <summary>
/// ���y�̓ǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"���y�ǂݍ��݃G���[",
			MB_OK
		);

		return FALSE;
	}

	//���̑��ݒ�
	audio->Volume = volume;
	audio->playType = playType;

	return TRUE;
}


/// <summary>
/// �摜�̓ǂݍ���
/// </summary>
/// <param name="img">�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImg(IMAGE* img,const char* path)
{
	strcpyDx(img->path,path );
	img->handle = LoadGraph(img->path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (img->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			img->path,			//���b�Z�[�W�^�C�g��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�{��
			MB_OK					//�{�^��
		);

		return FALSE;		//�G���[�I��
	}
	
	GetGraphSize(img->handle, &img->width, &img->height);

	return TRUE;
}