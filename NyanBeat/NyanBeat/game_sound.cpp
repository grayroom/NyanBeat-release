#include "game_sound.h"


void Init_Sound()	//	sound포인터에 각각의 사운드를 연결
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, MAX_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	FMOD_System_CreateSound(g_System, "../source/sound/sound_main.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[MAIN]);

	FMOD_System_CreateSound(g_System, "../source/sound/sound01.mp3", FMOD_LOOP_OFF, 0, &g_Sound[SONG_01]);
	FMOD_System_CreateSound(g_System, "../source/sound/sound02.mp3", FMOD_LOOP_OFF, 0, &g_Sound[SONG_02]);
	FMOD_System_CreateSound(g_System, "../source/sD:\Code\resource\libs\fmod\x86ound/sound03.mp3", FMOD_LOOP_OFF, 0, &g_Sound[SONG_03]);

	FMOD_System_CreateSound(g_System, "../source/sound/sound_hit.wav", FMOD_LOOP_OFF, 0, &g_Sound[HIT_NORMAL]);
	FMOD_Channel_SetVolume(g_Channel[HIT_NORMAL], VOLUME_NORMAL_HIT);
	FMOD_System_CreateSound(g_System, "../source/sound/sound_hit.wav", FMOD_LOOP_OFF, 0, &g_Sound[HIT_NICE]);
	FMOD_Channel_SetVolume(g_Channel[HIT_NICE], VOLUME_NICE_HIT);

	FMOD_System_CreateSound(g_System, "../source/sound/sound_result.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[GAME_RESULT]);
	FMOD_Channel_SetVolume(g_Channel[HIT_NICE], VOLUME_GAME_RESULT);
}

void Release_ALL_Sound()	//	선언한 모든 사운드와 시스템을 반환
{
	for(int i = 0; MAX_SOUND_CHANNEL > i; i++)
		FMOD_Sound_Release(g_Sound[i]);
	
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}

void Play_Sound(int selected_song, int* sound_channel)	//	입력받은 타입에 사운드를 대응하는 채널에 재생
{
	switch (selected_song)
	{
	case MAIN		: FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[MAIN], 0, &g_Channel[MAIN]); *sound_channel = MAIN;							break;
	case SONG_01	: FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[SONG_01], 0, &g_Channel[SONG_01]); *sound_channel = SONG_01;					break;
	case SONG_02	: FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[SONG_02], 0, &g_Channel[SONG_02]); *sound_channel = SONG_02;					break;
	case SONG_03	: FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[SONG_03], 0, &g_Channel[SONG_03]); *sound_channel = SONG_03;					break;
	case GAME_RESULT: FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[GAME_RESULT], 0, &g_Channel[GAME_RESULT]); *sound_channel = GAME_RESULT;		break;
	default: printf("ERROR CODE 31 : 사운드 채널이 없습니다!");																									break;
	}
}

void Stop_Channel(int sound_channel)	// 입력받은 채널의 사운드를 종료
{
	switch (sound_channel)
	{
	case MAIN		: FMOD_Channel_Stop(g_Channel[MAIN]);			break;
	case SONG_01	: FMOD_Channel_Stop(g_Channel[SONG_01]);		break;
	case SONG_02	: FMOD_Channel_Stop(g_Channel[SONG_02]);		break;
	case SONG_03	: FMOD_Channel_Stop(g_Channel[SONG_03]);		break;
	case GAME_RESULT: FMOD_Channel_Stop(g_Channel[GAME_RESULT]);	break;
	}

	return;
}