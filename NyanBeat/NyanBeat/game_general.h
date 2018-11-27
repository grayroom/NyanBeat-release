#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <fmod.h>

/*
[매크로 상수 정의]
[
	[이름]
	WINDOWSIZE_

	[사용처]
	game_general.c
	setup.c
]
*/
#define WINDOWSIZE_X	83		/*X축 - 콘솔창 크기*/
#define WINDOWSIZE_Y	35		/*Y축 - 콘솔창 크기*/
/* [정의 끝] */

/*
[매크로 상수 정의]
[
	[이름]
	XY_CURSORS

	[설명]
	그래픽 출력을 위한 위치를 지정하는 커서 매크로 상수입니다.
	함수 : void Move_Cursor(int x, int y)에서 사용됩니다.

	[주의]
	앞서 언급한 WINDOWSIZE 상수에 대하여 아래 상수들의 크기는, 0 <= CURSOR_X_ <= WINDOWSIZE_X, 0 <= CURSOR_Y_ <= WINDOWSIZE_Y 입니다.

	[사용처]
	game_general.c
	setup.c
]
*/
#define CURSOR_X_TITLE_MAIN			5	/*X축 - 메인화면 타이틀 출력.*/
#define CURSOR_Y_TITLE_MAIN			1	/*Y축 - 메인화면 타이틀 출력.*/

#define CURSOR_X_SELECT_MAIN		33	/*X축 - 메인화면 선택창 출력*/
#define CURSOR_Y_SELECT_MAIN		11	/*Y축 - 메인화면 선택창 출력*/

#define CURSOR_X_NYANCAT			0	/*X축 - 메인화면 이미지 출력*/
#define CURSOR_Y_NYANCAT			16	/*Y축 - 메인화면 이미지 출력*/

#define CURSOR_X_TITLE_SETTING		12	/*X축 - 설정화면 타이틀 출력*/
#define CURSOR_Y_TITLE_SETTING		1	/*Y축 - 설정화면 타이틀 출력*/
#define CURSOR_X_SELECT_SETTING		34	/*X축 - 설정화면 선택창 출력*/
#define CURSOR_Y_SELECT_SETTING		11	/*Y축 - 설정화면 선택창 출력*/

#define CURSOR_X_TITLE_SETTING_BG	1	/*X축 - 배경 설정화면 타이틀 출력*/
#define CURSOR_Y_TITLE_SETTING_BG	1	/*Y축 - 배경 설정화면 타이틀 출력*/
#define CURSOR_X_SELECT_SETTING_BG	13	/*X축 - 배경 설정화면 선택창 출력*/
#define CURSOR_Y_SELECT_SETTING_BG	11	/*Y축 - 배경 설정화면 선택창 출력*/

#define CURSOR_X_TITLE_SETTING_NT	1	/*X축 - 커서 설정화면 타이틀 출력*/
#define CURSOR_Y_TITLE_SETTING_NT	1	/*Y축 - 커서 설정화면 타이틀 출력*/
#define CURSOR_X_SELECT_SETTING_NT	37	/*X축 - 커서 설정화면 선택창 출력*/
#define CURSOR_Y_SELECT_SETTING_NT	11	/*Y축 - 커서 설정화면 선택창 출력*/

#define CURSOR_X_TITLE_SELECT_SONG	14	/*X축 - 노래 선택화면 타이틀 출력*/
#define CURSOR_Y_TITLE_SELECT_SONG	1	/*Y축 - 노래 선택화면 타이틀 출력*/
#define CURSOR_X_SELECT_SONG		21	/*X축 - 노래 선택화면 선택창 출력*/
#define CURSOR_Y_SELECT_SONG		18	/*Y축 - 노래 선택화면 선택창 출력*/

#define CURSOR_X_GAME				24	/*X축 - 게임 실행화면 구성*/
#define CURSOR_Y_GAME				10	/*Y축 - 게임 실행화면 구성*/

#define CURSOR_X_NOTE				4	/*X축 - 노트의 X축 간격 조절*/
#define CURSOR_Y_NOTE				10	/*Y축 - 노트의 Y축 간격 조절*/
#define CURSOR_X_GAGUE				30	/*X축 - 게이지 출력 위치 조절*/
#define CURSOR_Y_GAGUE				13	/*Y축 - 게이지 출력 위치 조절*/

#define CURSOR_X_TITLE_RESULT		4	/*Y축 - 결과화면 타이틀 출력*/
#define CURSOR_Y_TITLE_RESULT		1	/*Y축 - 결과화면 타이틀 출력*/
#define CURSOR_X_RESULT				42	/*X축 - 결과화면 상세결과 출력*/
#define CURSOR_X_RESULT_SIZE01		20	/*X축 - 결과화면 상세결과 출력 - 좌측에 표시되는 항목 정렬*/
#define CURSOR_X_RESULT_SIZE02		8	/*X축 - 결과화면 상세결과 출력 - 우측에 표시되는 항목 정렬*/
#define CURSOR_Y_RESULT				11	/*Y축 - 결과화면 상세결과 출력*/
#define CURSOR_Y_RESULT_SIZE		5	/*Y축 - 결과화면 상세결과 출력 - 결과창 좌우 구분자의 상하 높이 조절*/
#define CURSOR_X_SELECT_RESULT		31	/*X축 - 결과화면 선택창 출력*/
#define CURSOR_Y_SELECT_RESULT		19	/*Y축 - 결과화면 선택창 출력*/
/* [정의 끝] */

/*
[매크로 상수 정의]
[
	[이름]
	ETC
]
*/
#define GAUGE_MEASURE	13		/*게이지 크기 조절*/
#define PERCENT_MEASURE	100		/*비율을 백분율로 변경*/

#define MENU_END		13		/*메뉴를 끝내는 입력(ENTER)*/

#define SYNC_DELAY		1020	/*게임 시작시에 SYNC조절*/

#define COMBO_MEASURE	50		/*콤보에 따른 게이지 출력*/
#define COMBO_SCORE		1.025	/*콤보에 따른 추가점수 계산*/
/* [정의 끝] */

/*
[열거 상수]
[
	[설명]
	콘솔창에 출력할 문자의 색상을 지정해주는 열거 상수입니다.
	함수 : SetConsoleTextAttribute(_In_ HANDLE hConsoleOutput, _In_ WORD wAttributes)에서 사용됩니다.

	[사용처]
	game_general.c
]
*/
typedef enum
{
	Dark_Black,
	Dark_Blue,
	Dark_Green,
	Dark_Sky,
	Dark_Red,
	Dark_Purple,
	Dark_Yellow,
	Dark_White,
	Bright_Black,
	Bright_Blue,
	Bright_Green,
	Bright_Sky,
	Bright_Red,
	Bright_Purple,
	Bright_Yellow,
	Bright_White
} COLOR;

/*
[열거 상수]
[
	[설명]
	사용자가 입력한 key에대한 판정에 사용됩니다.
	구조체 : RESULT(int	hit[3]; int	combo_max; double score; double	note_percent; double combo_percent;)에서 사용됩니다.
}에서 사용됩니다.

	[사용처]
	game_general.c
]
*/
typedef enum
{
	nice,
	normal,
	miss
} HIT;

/*
[열거 상수]
[
	[설명]
	게임 종료시 선택에 사용됩니다.

	[사용처]
	game_general.c
	setup.c
	main.c
]
*/
typedef enum
{
	play_more,
	goto_main,
	exit_game
} MENU_FINAL;

/*
[구조체]
[
	[설명]
	게임실행중 게임 진행상황에 대한 정보를 저장하여 결과를 출력하는데 사용됩니다.

	[SEE_ALSO]
	game_general.c
]
*/
typedef struct _result
{
	int		hit[3];			/*사용자가 입력한 key에 대한 판정을 저장 - 열거형 HIT 태그를 이용하여 구별*/
	int		combo_max;		/*게임 진행중 최대 콤보를 저장*/
	double	score;			/*게임 진행중 현재 점수를 저장*/
	double	note_percent;	/*게임 진행중 총 노트수에 대한 정확히 입력한 key에대한 비율을 저장*/
	double	combo_percent;	/*결과창에서 MAX COMBO 강조를 위해 사용*/
} RESULT;

/* ========================================================================================== */
/* 함수 프로토타입                                                                            */
/* ========================================================================================== */

/*
	game_general.c에서 정의된 함수
*/

int		Play_MainFrame(FILE* note_ptr, int sound_channel, int note_type);
void	Hide_Cursor();
void	Move_Cursor(int x, int y);
void	Draw_Note(int note_type, int posit, int note);
void	Draw_Input(int posit);
void	Print_Note(int note_type, int note, int posit_x, int posit_y);
void	Print_Input(int posit_x, int posit_y);
int		Check_Point(int note_input[], int note_ref[], int *combo, int* combo_max, bool is_combo[], int* combo_level, int* result_hit);
void	Draw_Gauge(int* result_hit, int tot_note);
void	Clear_Note();
int		Show_Result(RESULT result, int tot_note);
void	Print_Result_Frame();
void	Print_Result(RESULT result, int tot_note);
void	Select_Menu_Result(int* menu);

/*
	game_sound.c에서 정의된 함수
*/

void	Init_Sound();
void	Release_ALL_Sound();
void	Play_Sound(int selected_song, int* sound_channel);
void	Stop_Channel(int sound_channel);

/*
	setup.c에서 정의된 함수
*/

int		Select_File(FILE** note_ptr);
void	Play_Game(FILE* note_ptr, int background_type, int note_type, int* final_choice);
void	Print_Title();
void	Draw_Background(int type);
int		Menu();
void	Setup(int* background_type, int* note_type);
void	Setup_Background(int* background_type);
void	Setup_Note_Type(int* note_type);