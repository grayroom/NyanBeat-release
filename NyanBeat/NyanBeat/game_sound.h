#define _CRT_SECURE_NO_WARNINGS

#include <fmod.h>

#pragma comment(lib, "fmodex_vc.lib")

/*
[매크로 상수 정의]
[
	[이름]
	SOUND

	[설명]
	사운드의 볼륨조절, 채널관리 등에 사용됨

	[사용처]
	game_sound.c
]
*/
#define MAX_SOUND			10
#define MAX_SOUND_CHANNEL	10
#define VOLUME_NORMAL_HIT	0.25
#define VOLUME_NICE_HIT		0.35
#define VOLUME_GAME_RESULT	0.3
/* [정의 끝] */

/*
[열거 상수]
[
	[설명]
	각각의 사운드에 대한 태그로 사용됨

	[사용처]
	game_general.c
	game_sound.c
	main.c
]
*/
typedef enum
{
	MAIN,		/*메인메뉴 배경음악*/
	SONG_01,	/*1번노래*/
	SONG_02,	/*2번노래*/
	SONG_03,	/*3번노래*/
	HIT_NORMAL,	/*획득한 점수가 01점~05점일때 효과음*/
	HIT_NICE,	/*획득한 점수가 05점~10점일때 효과음*/
	GAME_RESULT	/*결과창 배경음악*/
} SOUND;

/*
[전역변수]
[
	[설명]
	각각의 사운드를 사용하는데 이용

	[사용처]
	game_general.c
	game_sound.c
]
*/
FMOD_SYSTEM		*g_System;
FMOD_SOUND		*g_Sound[MAX_SOUND];
FMOD_CHANNEL	*g_Channel[MAX_SOUND_CHANNEL];

/* ========================================================================================== */
/* 함수 프로토타입                                                                            */
/* ========================================================================================== */

/*
	game_sound.c 에서 정의된 함수
*/

void		Init_Sound();
void		Play_Sound(int selected_song, int* sound_channel);
void		Release_Sound();
void		Stop_Channel(int sound_channel);