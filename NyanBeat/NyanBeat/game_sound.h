#define _CRT_SECURE_NO_WARNINGS

#include <fmod.h>

#pragma comment(lib, "fmodex_vc.lib")

/*
[��ũ�� ��� ����]
[
	[�̸�]
	SOUND

	[����]
	������ ��������, ä�ΰ��� � ����

	[���ó]
	game_sound.c
]
*/
#define MAX_SOUND			10
#define MAX_SOUND_CHANNEL	10
#define VOLUME_NORMAL_HIT	0.25
#define VOLUME_NICE_HIT		0.35
#define VOLUME_GAME_RESULT	0.3
/* [���� ��] */

/*
[���� ���]
[
	[����]
	������ ���忡 ���� �±׷� ����

	[���ó]
	game_general.c
	game_sound.c
	main.c
]
*/
typedef enum
{
	MAIN,		/*���θ޴� �������*/
	SONG_01,	/*1���뷡*/
	SONG_02,	/*2���뷡*/
	SONG_03,	/*3���뷡*/
	HIT_NORMAL,	/*ȹ���� ������ 01��~05���϶� ȿ����*/
	HIT_NICE,	/*ȹ���� ������ 05��~10���϶� ȿ����*/
	GAME_RESULT	/*���â �������*/
} SOUND;

/*
[��������]
[
	[����]
	������ ���带 ����ϴµ� �̿�

	[���ó]
	game_general.c
	game_sound.c
]
*/
FMOD_SYSTEM		*g_System;
FMOD_SOUND		*g_Sound[MAX_SOUND];
FMOD_CHANNEL	*g_Channel[MAX_SOUND_CHANNEL];

/* ========================================================================================== */
/* �Լ� ������Ÿ��                                                                            */
/* ========================================================================================== */

/*
	game_sound.c ���� ���ǵ� �Լ�
*/

void		Init_Sound();
void		Play_Sound(int selected_song, int* sound_channel);
void		Release_Sound();
void		Stop_Channel(int sound_channel);