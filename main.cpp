
//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include"keyboard.h"	//�L�[�{�[�h�̏���
#include"FPS.h"			//FPS�̏���

#include <math.h>		//���w

//�}�N����`
#define TAMA_DIV_MAX	4
#define TAMA_MAX	100
#define TEKI_KIND	8
#define TEKI_MAX	10

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

struct TAMA {
	int handle[TAMA_DIV_MAX];		//�摜�n���h��
	char path[255];		//�摜�̃p�X

	int DivTate;		//�������i�c�j
	int DivYoko;		//�������i���j
	int DivMax;			//��������

	int AnimeCnt = 0;		//�A�j���[�V�����J�E���^
	int AnimeCntMAX = 0;	//�A�j���[�V�����J�E���^MAX

	int NowIndex = 0;		//���݂̉摜�̗v�f��

	int StartX;			//X�̏����ʒu
	int StartY;			//Y�̏����ʒu

	int radius;			//���a
	int degree;			//�p�x

	int x ;			//X�ʒu
	int y ;			//Y�ʒu
	int width ;		//����
	int height;		//��

	int speed;

	RECT coll;		//�����蔻��

	BOOL IsDraw = FALSE;		//�`��ł���H
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

//�e�̍\����
struct TAMA tama_moto;		//��
struct TAMA tama[TAMA_MAX];	//���ۂɎg����

//�e�̍\����
int tamaShotCnt = 0;
int tamaShotCntMAX = 5;

//�v���C���[
CHARCTOR player;

//�w�i
IMAGE back[2];

//�G
CHARCTOR enemy_moto[TEKI_KIND];

//���ۂ̓G�f�[�^
CHARCTOR enemy[TEKI_MAX];

char enemy_path[TEKI_KIND][255] =
{
	{".\\image\\enemy_blue.png"},
	{".\\image\\enemy_gray.png"},
	{".\\image\\enemy_green.png"},
	{".\\image\\enemy_skyblue.png"},
	{".\\image\\enemy_purple.png"},
	{".\\image\\enemy_red.png"},
	{".\\image\\enemy_red_big.png"},
	{".\\image\\enemy_yellow.png"}
};

int TekiAddCnt = 0;
int TekiAddCntMax = 60;

//�Q�[���X�R�A
int Score = 0;

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

VOID CollUpdatePlayer(CHARCTOR* chara);	//�����蔻��̗̈���X�V�i�v���C���[�j
VOID CollUpdate(CHARCTOR* chara);	//�����蔻��
VOID CollUpdateEnemy(CHARCTOR* chara);	//�����蔻��̗̈���X�V�i�G�j

VOID TamaCollUpdate(TAMA* tama);	//�e�̓����蔻��

BOOL OnCollrect(RECT a, RECT b);	//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̓ǂݍ���
VOID GameInit(VOID);

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//���y�̓ǂݍ���
BOOL LoadImg(IMAGE* img, const char* path);	//�摜�̓ǂݍ���

BOOL LoadImageDivMem(int* handle, const char* path, int divyoko, int divtate);	//�摜�𕪊����ēǂݍ���

VOID DrawTama(TAMA* tama);		//�e�̕`��
VOID ShotTama(TAMA* tama, float deg,int X);	//�e�̔���

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

	//�ǂݍ��񂾉摜���J��
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }

	//�v���C���[�摜�̉��
	DeleteGraph(player.img.handle);

	//�w�i�摜�̉��
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);

	for (int i = 0; i < TEKI_KIND; i++)
	{
		DeleteGraph(enemy[1].img.handle);
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
	//�e�̕�������ݒ�
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//�e�̃p�X���R�s�[
	strcpyDx(tama_moto.path, ".\\image\\dia_green.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadImageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	//���ƍ������擾
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//�ʒu��ݒ�
	tama_moto.x = GAME_WIDTH / 2 + tama_moto.width / 2;
	tama_moto.y = GAME_HEIGHT / 2 + tama_moto.height / 2;

	tama_moto.speed = 10;		//���x

	//�A�j����ς��鑬�x
	tama_moto.AnimeCntMAX = 15;

	//�����蔻��̍X�V
	TamaCollUpdate(&tama_moto);

	//�摜��\�����Ȃ�
	tama_moto.IsDraw = FALSE;

	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//�v���C���[�̉摜��ǂݍ���
	if (LoadImg(&player.img, ".\\image\\player.png") == FALSE) { return FALSE; }
	player.img.x = GAME_WIDTH / 2 + player.img.width / 2;
	player.img.y = GAME_HEIGHT / 2 + player.img.height / 2;
	CollUpdatePlayer(&player);	//�����蔻��̍X�V
	player.img.IsDraw = TRUE;	//�`�悷��

	player.speed = 10;
	
	//�w�i�摜��ǂݍ��݇@
	if (LoadImg(&back[0], ".\\image\\hosi.png"));
	back[0].x = 0;
	back[0].y = -back[0].height;
	back[0].IsDraw = TRUE;	//�`�悷��
	
	//�w�i�摜��ǂݍ��݇A
	if (LoadImg(&back[1], ".\\image\\hosi_rev.png"));
	back[1].x = 0;
	back[1].y = 0;
	back[1].IsDraw = TRUE;	//�`�悷��

	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImg(&enemy_moto[i].img, enemy_path[i]) == FALSE) { return FALSE; }
		enemy_moto[i].img.x = 0;
		enemy_moto[i].img.y = 0;
		CollUpdatePlayer(&enemy_moto[i]);
		enemy_moto[i].img.IsDraw = FALSE;	//�`�悷��
	}

	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�Q�[���X�R�A�̏�����
	Score = 0;

	//�v���C���[�̏�����
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;
	CollUpdatePlayer(&player);	//�����蔻��̍X�V
	player.img.IsDraw = TRUE;	//�`�悷��

	//�w�i�摜�̏������@
	back[0].x = 0;
	back[0].y = -back[0].height;
	back[0].IsDraw = TRUE;	//�`�悷��

	//�w�i�摜�̏������A
	back[1].x = 0;
	back[1].y = 0;
	back[1].IsDraw = TRUE;	//�`�悷��

	//�G�̏�����
	for (int i = 0; i < TEKI_KIND; i++)
	{
		enemy_moto[i].img.x = 0;
		enemy_moto[i].img.y = 0;
		CollUpdatePlayer(&enemy_moto[i]);
		enemy_moto[i].img.IsDraw = FALSE;	//�`�悷��
	}
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

	//�v���C���[����
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.coll.left >= 0)
		{
			player.img.x -= player.speed;
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.coll.right <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.coll.top >= 0)
		{
			player.img.y -= player.speed;
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.coll.bottom <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}

	CollUpdatePlayer(&player);	//�����蔻��̍X�V

	//�X�y�[�X�L�[�������Ă���Ƃ�
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			//�e�𔭎˂���
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 240.0f, -20);

					break;
				}
			}
			//�e�𔭎˂���
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 270.0f, -10);

					break;
				}
			}
			//�e�𔭎˂���
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 270.0f, 10);

					break;
				}
			}
			//�e�𔭎˂���
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 300.0f, 20);

					break;
				}
			}
		}
		//�e�̔��ˑ҂�
		if (tamaShotCnt < tamaShotCntMAX)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}


	//�e���΂�
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (tama[i].IsDraw == TRUE)
		{
			//�e�̈ʒu���C��
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius += tama[i].speed;

			//�����蔻��̍X�V
			TamaCollUpdate(&tama[i]);

			//��ʊO�ɏo����`�悵�Ȃ�
			if (
				tama[i].y + tama[i].height < 0 ||	//��
				tama[i].y > GAME_HEIGHT||			//��
				tama[i].x + tama[i].width < 0 ||	//��
				tama[i].x > GAME_WIDTH				//�E
				)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (TekiAddCnt < TekiAddCntMax)
	{
		TekiAddCnt++;
	}
	else
	{
		TekiAddCnt = 0;

		//�G�𐶐�
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悳��Ă��Ȃ��G��T��
			if (enemy[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 10;	//��ʂ̉�������

				if (Score < 1000)
				{
					enemy[i] = enemy_moto[0];
				}
				else if (Score < 2000)
				{
					enemy[i] = enemy_moto[0];
				}
				else
				{
					enemy[i] = enemy_moto[GetRand(TEKI_KIND - 1)];
				}

				enemy[i].img.x = GetRand(Bunkatu = 1) * GAME_WIDTH / Bunkatu;
				enemy[i].img.y = -enemy[i].img.height;

				enemy[i].img.IsDraw = TRUE;	//�`�悷��
				break;
			}
		}
	}

	//�G�̏���
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�`�悳��Ă���G
		if (enemy[i].img.IsDraw == TRUE)
		{
			enemy[i].img.y += 1;

			//�G�̓����蔻��̍X�V
			CollUpdateEnemy(&enemy[i]);

			//�G����ʔ����ɍs������\�����Ȃ�
			if (enemy[i].img.y > GAME_HEIGHT)
			{
				enemy[i].img.IsDraw = FALSE;
			}

			//�G�Ǝ����̋����������Ƃ�
			for (int cnt = 0; cnt < TAMA_MAX; cnt++)
			{
				if(tama[cnt].IsDraw == TRUE)
				//�����蔻��
				if (OnCollrect(enemy[i].coll, tama[cnt].coll) == TRUE)
				{
					tama[i].IsDraw = FALSE;
					enemy[i].img.IsDraw = FALSE;

					Score += 100;
				}
			}
		}
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{	
	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�`��
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//�摜�����܂ōs�����Ƃ�
		if (back[i].y > GAME_HEIGHT - 1)
		{
			back[i].y = -back[i].height + 1;
		}

		//�摜�����ɓ�����
		back[i].y += 3;
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		if (enemy[i].img.IsDraw == TRUE)
		{
			DrawGraph(enemy[i].img.x, enemy[i].img.y, enemy[i].img.handle, TRUE);
		}

		//�����蔻��̕`��
		if (Game_DEBUG == TRUE)
		{
			DrawBox(enemy[i].coll.left, enemy[i].coll.top, enemy[i].coll.right, enemy[i].coll.bottom,
				GetColor(0, 0, 255), FALSE);
		}
	}

	//�v���C���[�̕`��
	if (player.img.IsDraw = TRUE)
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		if (Game_DEBUG == TRUE)
		{
			DrawBox(
				player.coll.left,
				player.coll.top,
				player.coll.right,
				player.coll.bottom,
				GetColor(255, 0, 0),
				FALSE
			);
		}
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if(tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			if (Game_DEBUG == TRUE)
			{
				DrawBox(
					tama[i].coll.left,
					tama[i].coll.top,
					tama[i].coll.right,
					tama[i].coll.bottom,
					GetColor(255, 0, 0),
					FALSE
				);
			}
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();
	SetFontSize(40);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE:%05d", Score);
	SetFontSize(old);

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
	chara->coll.left = chara->img.x + 50;
	chara->coll.top = chara->img.y + 45;
	chara->coll.right = chara->img.x + chara->img.width - 50;
	chara->coll.bottom = chara->img.y + chara->img.height - 45;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�G�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdateEnemy(CHARCTOR* chara)
{
	chara->coll.left = chara->img.x + 10;
	chara->coll.top = chara->img.y + 10;
	chara->coll.right = chara->img.x + chara->img.width - 10;
	chara->coll.bottom = chara->img.y + chara->img.height - 15;

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
/// �e�̓����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�e�̓����蔻��̗̈�</param>
VOID TamaCollUpdate(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

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
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="divyoko">��������Ƃ��̉��̐�</param>
/// <param name="divtate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(int *handle ,const char *path,int divyoko,int divtate)
{

	//�e�̓ǂݍ���
	int IsTamaLoad = -1;	//�摜���ǂݍ��߂����H

	//�ꎞ�I�ɉ摜�n���h����p�ӂ���
	int TamaHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			path,
			"�摜�ǂݍ��݃G���[",
			MB_OK
		);

		return FALSE;
	}

	//�摜�̕��ƍ������擾
	int TamaWidth = -1;
	int TamaHeight = -1;
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//�������ēǂݍ���
	IsTamaLoad = LoadDivGraph(
		path,					//�摜�̃p�X
		TAMA_DIV_MAX,			//��������
		divyoko, divtate,		//���̕����A�c�̕���
		TamaWidth / divyoko, TamaHeight / divtate,	//�摜�P���̕��A����
		handle);				//�A���ŊǗ�����z��̐擪�A�h���X

	//�摜�����G���[
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			path,
			"�摜�����G���[",
			MB_OK
		);

		return FALSE;
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�����J��
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// �摜���������ɓǂݍ���
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


/// <summary>
/// �e�̕`��
/// </summary>
/// <param name="tama">�e�̍\����</param>
/// <returns></returns>
VOID DrawTama(TAMA* tama)
{
	//�e�̕`�悪�ł���Ƃ�
	if (tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//�e�̓Y�������e�̕������̍ő�����������Ƃ�
			if (tama->NowIndex < tama->DivTate * tama->DivYoko - 1)
			{
				tama->NowIndex++;
			}
			else
			{
				tama->NowIndex = 0;
			}

			tama->AnimeCnt = 0;
		}
	}
}

/// <summary>
/// �e���΂�
/// </summary>
VOID ShotTama(TAMA* tama, float deg,int X)
{
		//�e�̔���
		tama -> IsDraw = TRUE;

		//�e�̈ʒu
		tama -> StartX = player.img.x + player.img.width / 2 - tama -> width / 2 + X;
		tama -> StartY = player.img.y + 25;

		//�e�̑��x��ς���
		tama -> speed = 10;

		//�e�̊p�x
		tama -> degree = deg;

		//�e�̔��a
		tama -> radius = 0.0f;

		//�e�̓����蔻��̍X�V
		TamaCollUpdate(tama);
}