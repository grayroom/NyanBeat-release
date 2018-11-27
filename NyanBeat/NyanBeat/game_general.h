#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <fmod.h>

/*
[��ũ�� ��� ����]
[
	[�̸�]
	WINDOWSIZE_

	[���ó]
	game_general.c
	setup.c
]
*/
#define WINDOWSIZE_X	83		/*X�� - �ܼ�â ũ��*/
#define WINDOWSIZE_Y	35		/*Y�� - �ܼ�â ũ��*/
/* [���� ��] */

/*
[��ũ�� ��� ����]
[
	[�̸�]
	XY_CURSORS

	[����]
	�׷��� ����� ���� ��ġ�� �����ϴ� Ŀ�� ��ũ�� ����Դϴ�.
	�Լ� : void Move_Cursor(int x, int y)���� ���˴ϴ�.

	[����]
	�ռ� ����� WINDOWSIZE ����� ���Ͽ� �Ʒ� ������� ũ���, 0 <= CURSOR_X_ <= WINDOWSIZE_X, 0 <= CURSOR_Y_ <= WINDOWSIZE_Y �Դϴ�.

	[���ó]
	game_general.c
	setup.c
]
*/
#define CURSOR_X_TITLE_MAIN			5	/*X�� - ����ȭ�� Ÿ��Ʋ ���.*/
#define CURSOR_Y_TITLE_MAIN			1	/*Y�� - ����ȭ�� Ÿ��Ʋ ���.*/

#define CURSOR_X_SELECT_MAIN		33	/*X�� - ����ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_MAIN		11	/*Y�� - ����ȭ�� ����â ���*/

#define CURSOR_X_NYANCAT			0	/*X�� - ����ȭ�� �̹��� ���*/
#define CURSOR_Y_NYANCAT			16	/*Y�� - ����ȭ�� �̹��� ���*/

#define CURSOR_X_TITLE_SETTING		12	/*X�� - ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_Y_TITLE_SETTING		1	/*Y�� - ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_X_SELECT_SETTING		34	/*X�� - ����ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_SETTING		11	/*Y�� - ����ȭ�� ����â ���*/

#define CURSOR_X_TITLE_SETTING_BG	1	/*X�� - ��� ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_Y_TITLE_SETTING_BG	1	/*Y�� - ��� ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_X_SELECT_SETTING_BG	13	/*X�� - ��� ����ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_SETTING_BG	11	/*Y�� - ��� ����ȭ�� ����â ���*/

#define CURSOR_X_TITLE_SETTING_NT	1	/*X�� - Ŀ�� ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_Y_TITLE_SETTING_NT	1	/*Y�� - Ŀ�� ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_X_SELECT_SETTING_NT	37	/*X�� - Ŀ�� ����ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_SETTING_NT	11	/*Y�� - Ŀ�� ����ȭ�� ����â ���*/

#define CURSOR_X_TITLE_SELECT_SONG	14	/*X�� - �뷡 ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_Y_TITLE_SELECT_SONG	1	/*Y�� - �뷡 ����ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_X_SELECT_SONG		21	/*X�� - �뷡 ����ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_SONG		18	/*Y�� - �뷡 ����ȭ�� ����â ���*/

#define CURSOR_X_GAME				24	/*X�� - ���� ����ȭ�� ����*/
#define CURSOR_Y_GAME				10	/*Y�� - ���� ����ȭ�� ����*/

#define CURSOR_X_NOTE				4	/*X�� - ��Ʈ�� X�� ���� ����*/
#define CURSOR_Y_NOTE				10	/*Y�� - ��Ʈ�� Y�� ���� ����*/
#define CURSOR_X_GAGUE				30	/*X�� - ������ ��� ��ġ ����*/
#define CURSOR_Y_GAGUE				13	/*Y�� - ������ ��� ��ġ ����*/

#define CURSOR_X_TITLE_RESULT		4	/*Y�� - ���ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_Y_TITLE_RESULT		1	/*Y�� - ���ȭ�� Ÿ��Ʋ ���*/
#define CURSOR_X_RESULT				42	/*X�� - ���ȭ�� �󼼰�� ���*/
#define CURSOR_X_RESULT_SIZE01		20	/*X�� - ���ȭ�� �󼼰�� ��� - ������ ǥ�õǴ� �׸� ����*/
#define CURSOR_X_RESULT_SIZE02		8	/*X�� - ���ȭ�� �󼼰�� ��� - ������ ǥ�õǴ� �׸� ����*/
#define CURSOR_Y_RESULT				11	/*Y�� - ���ȭ�� �󼼰�� ���*/
#define CURSOR_Y_RESULT_SIZE		5	/*Y�� - ���ȭ�� �󼼰�� ��� - ���â �¿� �������� ���� ���� ����*/
#define CURSOR_X_SELECT_RESULT		31	/*X�� - ���ȭ�� ����â ���*/
#define CURSOR_Y_SELECT_RESULT		19	/*Y�� - ���ȭ�� ����â ���*/
/* [���� ��] */

/*
[��ũ�� ��� ����]
[
	[�̸�]
	ETC
]
*/
#define GAUGE_MEASURE	13		/*������ ũ�� ����*/
#define PERCENT_MEASURE	100		/*������ ������� ����*/

#define MENU_END		13		/*�޴��� ������ �Է�(ENTER)*/

#define SYNC_DELAY		1020	/*���� ���۽ÿ� SYNC����*/

#define COMBO_MEASURE	50		/*�޺��� ���� ������ ���*/
#define COMBO_SCORE		1.025	/*�޺��� ���� �߰����� ���*/
/* [���� ��] */

/*
[���� ���]
[
	[����]
	�ܼ�â�� ����� ������ ������ �������ִ� ���� ����Դϴ�.
	�Լ� : SetConsoleTextAttribute(_In_ HANDLE hConsoleOutput, _In_ WORD wAttributes)���� ���˴ϴ�.

	[���ó]
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
[���� ���]
[
	[����]
	����ڰ� �Է��� key������ ������ ���˴ϴ�.
	����ü : RESULT(int	hit[3]; int	combo_max; double score; double	note_percent; double combo_percent;)���� ���˴ϴ�.
}���� ���˴ϴ�.

	[���ó]
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
[���� ���]
[
	[����]
	���� ����� ���ÿ� ���˴ϴ�.

	[���ó]
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
[����ü]
[
	[����]
	���ӽ����� ���� �����Ȳ�� ���� ������ �����Ͽ� ����� ����ϴµ� ���˴ϴ�.

	[SEE_ALSO]
	game_general.c
]
*/
typedef struct _result
{
	int		hit[3];			/*����ڰ� �Է��� key�� ���� ������ ���� - ������ HIT �±׸� �̿��Ͽ� ����*/
	int		combo_max;		/*���� ������ �ִ� �޺��� ����*/
	double	score;			/*���� ������ ���� ������ ����*/
	double	note_percent;	/*���� ������ �� ��Ʈ���� ���� ��Ȯ�� �Է��� key������ ������ ����*/
	double	combo_percent;	/*���â���� MAX COMBO ������ ���� ���*/
} RESULT;

/* ========================================================================================== */
/* �Լ� ������Ÿ��                                                                            */
/* ========================================================================================== */

/*
	game_general.c���� ���ǵ� �Լ�
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
	game_sound.c���� ���ǵ� �Լ�
*/

void	Init_Sound();
void	Release_ALL_Sound();
void	Play_Sound(int selected_song, int* sound_channel);
void	Stop_Channel(int sound_channel);

/*
	setup.c���� ���ǵ� �Լ�
*/

int		Select_File(FILE** note_ptr);
void	Play_Game(FILE* note_ptr, int background_type, int note_type, int* final_choice);
void	Print_Title();
void	Draw_Background(int type);
int		Menu();
void	Setup(int* background_type, int* note_type);
void	Setup_Background(int* background_type);
void	Setup_Note_Type(int* note_type);