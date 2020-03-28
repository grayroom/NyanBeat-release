#include "game_general.h"
#include "game_sound.h"

int Select_File(FILE** note_ptr)	// 파일을 그래픽으로 구현된 메뉴에서 선택함
{
	FILE* pTitle_result;

	unsigned char str[100];

	int i = 0;

	int	note_select = 1;
	int	is_end = 0;
	int	input = 0;

	system("cls");

	pTitle_result = fopen("../source/title/title_select_song.txt", "r");

	while (feof(pTitle_result) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_SELECT_SONG, CURSOR_Y_TITLE_SELECT_SONG + i);

		fgets(str, 100, pTitle_result);
		printf("%s", str);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_SELECT_SONG - 2);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG);
	printf("▶1. (HARD) NyanCat - daniwell");
	Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + 1);
	printf("  2. (EASY) NyanCat - daniwell");
	Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + 2);
	printf("  3. (SUPER EASY) Electro mafia - JHShin");

	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (note_select)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("  "); break;
			case 3: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("  "); break;
			}

			if (input == 72)
			{
				if (note_select > 1)
					note_select -= 1;
			}
			else if (input == 80)
			{
				if (note_select < 3)
					note_select += 1;
			}
			else
				continue;

			switch (note_select)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("▶"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("▶"); break;
			case 3: Move_Cursor(CURSOR_X_SELECT_SONG, CURSOR_Y_SELECT_SONG + note_select - 1); printf("▶"); break;
			}
		}


	} while (is_end != MENU_END);

	switch (note_select)
	{
	case 1: *note_ptr = fopen("../source/note/note_01.txt", "r"); break;
	case 2: *note_ptr = fopen("../source/note/note_02.txt", "r"); break;
	case 3: *note_ptr = fopen("../source/note/note_03.txt", "r"); break;
	}

	while (_kbhit())
		_getch();

	return note_select;
}

void Play_Game(FILE* note_ptr, int background_type, int note_type, int* final_choice)	// 선택한 파일, 배경타입, 노트타입을 기반으로 사운드를 재생, 배경출력 후 MainFrame(게임 실제 가동부)을 실행함. 
{
	int		sound_channel = 0;

	int		selected_note = 0;

	clock_t	ticks_bigin;
	clock_t	ticks_last;


	selected_note = Select_File(&note_ptr);

	do
	{
		rewind(note_ptr);

		Draw_Background(background_type);

		ticks_bigin = clock();

		Play_Sound(selected_note, &sound_channel);

		do
		{
			ticks_last = clock();
		} while (ticks_last - ticks_bigin < SYNC_DELAY);


		*final_choice = Play_MainFrame(note_ptr, sound_channel, note_type);
	} while (*final_choice == play_more); // MainFrame에서 한번 더 플레이 하기를 선택했다면 이전에 선택한 옵션으로 게임을 다시 실행

	while (_kbhit())
		_getch();

	return;
}

void Print_Title()	// 메인 타이틀의 그래픽을 출력
{
	FILE* pTitle;
	FILE* pNyanCat;

	pTitle = fopen("../source/title/title_main.txt", "r");
	pNyanCat = fopen("../source/title/image_nyancat.txt", "r");

	unsigned char str[100];
	unsigned char nyancat[100];

	int i = 0;

	while (feof(pTitle) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_MAIN, CURSOR_Y_TITLE_MAIN + i);

		fgets(str, 100, pTitle);
		printf("%s", str);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_TITLE_MAIN + 8);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	Move_Cursor(0, CURSOR_Y_TITLE_MAIN + 14);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	i = 0;

	while (feof(pNyanCat) == 0)
	{
		Move_Cursor(CURSOR_X_NYANCAT, CURSOR_Y_NYANCAT + i);

		fgets(nyancat, 100, pNyanCat);
		printf("%s", nyancat);

		i++;
	}

	return;
}

void Draw_Background(int type)	//	게임 플레이화면의 배경프레임을 그림
{
	if (type == 1)
	{
		Sleep(500);
		system("cls");

		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME);
		printf("┌───────────────────────────┬─────┐");
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 1);
		printf("│ ┌───────┬───────┬───────┐ │     │");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 2 + i);
			printf("│ │       │       │       │ │     │");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 5);
		printf("│ ├───────┼───────┼───────┤ │     │");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 6 + i);
			printf("│ │       │       │       │ │     │");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 9);
		printf("│ ├───────┼───────┼───────┤ │     │");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 10 + i);
			printf("│ │       │       │       │ │     │");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 13);
		printf("│ └───────┴───────┴───────┘ │     │");
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 14);
		printf("└───────────────────────────┴─────┘");
	}

	else if (type == 2)
	{
		Sleep(500);
		system("cls");

		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME);
		printf("┌──────────────┬──┐\n");
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 1);
		printf("│ ┌───┬───┬───┐ │    │\n");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 2 + i);
			printf("│ │      │      │      │ │    │\n");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 5);
		printf("│ ├───┼───┼───┤ │    │\n");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 6 + i);
			printf("│ │      │      │      │ │    │\n");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 9);
		printf("│ ├───┼───┼───┤ │    │\n");
		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 10 + i);
			printf("│ │      │      │      │ │    │\n");
		}
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 13);
		printf("│ └───┴───┴───┘ │    │\n");
		Move_Cursor(CURSOR_X_GAME, CURSOR_Y_GAME + 14);
		printf("└──────────────┴──┘");
	}

	else
		printf("ERROR : 입력한 type이 유효하지 않습니다!");

	return;
}

int Menu()	//	메인메뉴를 출력 및 선택
{
	int	menu = 1;
	int	is_end = 0;
	int	input = 0;

	Print_Title();

	Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN);
	printf("▶1. 게임시작\n");
	Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + 1);
	printf("  2. 환경설정\n");
	Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + 2);
	printf("  3. 종료\n");

	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("  "); break;
			case 3: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("  "); break;
			}

			if (input == 72)
			{
				if (menu > 1)
					menu -= 1;
			}
			else if (input == 80)
			{
				if (menu < 3)
					menu += 1;
			}
			else
				continue;

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("▶"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("▶"); break;
			case 3: Move_Cursor(CURSOR_X_SELECT_MAIN, CURSOR_Y_SELECT_MAIN + menu - 1); printf("▶"); break;
			}
		}


	} while (is_end != MENU_END);

	while (_kbhit())
		_getch();

	system("cls");

	return menu;
}

void Setup(int* background_type, int* note_type)	// 설정메뉴를 출력 및 선택
{
	int	menu = 1;
	int	is_end = 0;
	int	input = 0;
	FILE* pTitle;

	pTitle = fopen("../source/title/title_setting.txt", "r");

	unsigned char title[100];

	int i = 0;

	while (feof(pTitle) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_SETTING, CURSOR_Y_TITLE_SETTING + i);

		fgets(title, 100, pTitle);
		printf("%s", title);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_TITLE_SETTING + 8);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	Move_Cursor(CURSOR_X_SELECT_SETTING - 5, CURSOR_Y_SELECT_SETTING);
	printf("설정 할 항목을 선택하세요");
	Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 2);
	printf("▶1. 배경프레임");
	Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 3);
	printf("  2. 노트 모양");


	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 1 + menu); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 1 + menu); printf("  "); break;
			}

			if (input == 72)
			{
				if (menu > 1)
					menu -= 1;
			}
			else if (input == 80)
			{
				if (menu < 2)
					menu += 1;
			}
			else
				continue;

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 1 + menu); printf("▶"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SETTING, CURSOR_Y_SELECT_SETTING + 1 + menu); printf("▶"); break;
			}
		}


	} while (is_end != MENU_END);

	if (_kbhit())
		_getch();

	system("cls");

	switch (menu)
	{
	case 1: Setup_Background(background_type); break;
	case 2: Setup_Note_Type(note_type); break;
	}

	while (_kbhit())
		_getch();

	return;
}

void Setup_Background(int* background_type)	//	배경 설정메뉴를 출력 및 선택
{

	int	menu = 1;
	int	is_end = 0;
	int	input = 0;
	FILE* pTitle;

	pTitle = fopen("../source/title/title_setting_background.txt", "r");

	unsigned char title[100];

	int i = 0;

	while (feof(pTitle) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_SETTING_BG, CURSOR_Y_TITLE_SETTING_BG + i);

		fgets(title, 100, pTitle);
		printf("%s", title);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_TITLE_SETTING_BG + 8);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	Move_Cursor(CURSOR_X_SELECT_SETTING_BG + 14, CURSOR_Y_SELECT_SETTING_BG);
	printf("Background Type을 선택하세요");
	Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 2);
	printf("▶1. 기본옵션");
	Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 3);
	printf("  2. 기본옵션이 정상적으로 작동하지 않을 때 선택하십시오");


	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 1 + menu); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 1 + menu); printf("  "); break;
			}

			if (input == 72)
			{
				if (menu > 1)
					menu -= 1;
			}
			else if (input == 80)
			{
				if (menu < 2)
					menu += 1;
			}
			else
				continue;

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 1 + menu); printf("▶"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SETTING_BG, CURSOR_Y_SELECT_SETTING_BG + 1 + menu); printf("▶"); break;
			}
		}


	} while (is_end != MENU_END);

	if (_kbhit())
		_getch();

	system("cls");

	*background_type = menu;

	while (_kbhit())
		_getch();

	return;
}

void Setup_Note_Type(int* note_type)	//	노트타입 설정메뉴를 출력 및 선택
{
	int menu = 1;
	int	is_end = 0;
	int	input = 0;

	FILE* pTitle;

	pTitle = fopen("../source/title/title_setting_notetype.txt", "r");

	unsigned char title[100];

	int i = 0;

	while (feof(pTitle) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_SETTING_NT, CURSOR_Y_TITLE_SETTING_NT + i);

		fgets(title, 100, pTitle);
		printf("%s", title);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_SELECT_SETTING_NT - 2);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	Move_Cursor(CURSOR_X_SELECT_SETTING_NT - 7, CURSOR_Y_SELECT_SETTING_NT);
	printf("Note Type을 선택하세요");
	Move_Cursor(CURSOR_X_SELECT_SETTING_NT - 12, CURSOR_Y_SELECT_SETTING_NT + 1);
	printf("(팁 : 커서를 이동시켜서 미리보기)");
	Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 3);
	printf("▶ type01");
	Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 8);
	printf("   type02");

	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (menu)
			{
			case 1: Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 3); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 8); printf("  "); break;
			}

			if (input == 72)
			{
				if (menu > 1)
					menu -= 1;
			}
			else if (input == 80)
			{
				if (menu < 2)
					menu += 1;
			}
			else
				continue;

			switch (menu)
			{
			case 1:
				Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 3); printf("▶");

				for (int i = 1; i < 10; i++)
				{
					Print_Note(menu, i, CURSOR_X_SELECT_SETTING_NT + 3, CURSOR_Y_SELECT_SETTING_NT + 4);
					Sleep(100);
				}
				break;
			case 2:
				Move_Cursor(CURSOR_X_SELECT_SETTING_NT, CURSOR_Y_SELECT_SETTING_NT + 8); printf("▶");

				for (int i = 1; i < 10; i++)
				{
					Print_Note(menu, i, CURSOR_X_SELECT_SETTING_NT + 3, CURSOR_Y_SELECT_SETTING_NT + 9);
					Sleep(100);
				}
				break;
			}


		}


	} while (is_end != MENU_END);

	if (_kbhit())
		_getch();

	system("cls");

	*note_type = menu;

	while (_kbhit())
		_getch();

	return;
}