
#include "game_general.h"
#include "game_sound.h"

int Play_MainFrame(FILE* note_ptr, int sound_channel, int note_type)
{
	char			metadata_line[10] = { 0, };
	char			metadata_rate[10] = { 0, };
	char			metadata_note[10] = { 0, };
	char			trashbin = 0;

	int				load_data[9] = { 0, };

	int				tot_line = 0;
	int				frame_rate = 0;
	int				tot_note = 0;

	int				note_input[2] = { 0, };
	int				note_at_once = 0;

	int				point_add = 0;

	int				combo = 0;
	int				combo_level = 0;
	bool			is_combo[9] = { FALSE, };

	int				final_choice = 0;

	RESULT			result;
					memset(&result, 0, sizeof(result));

	clock_t			delay_input = 0;

	clock_t			ticks_bigin;
	clock_t			ticks_mid;
	clock_t			ticks_last;


	// 메타데이터 로드
	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_line[col]);
	fscanf(note_ptr, "%1c", &trashbin);	// %1c로 입력받을경우 WhiteSpace를 걸러낼 수 없으므로 사용

	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_rate[col]);
	fscanf(note_ptr, "%1c", &trashbin);

	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_note[col]);
	fscanf(note_ptr, "%1c", &trashbin);

	tot_line = atoi(metadata_line);
	frame_rate = atoi(metadata_rate);
	tot_note = atoi(metadata_note);

	delay_input = CLOCKS_PER_SEC / (2 * frame_rate);	// 메타데이터에서 로드한 프레임률(fps)를 딜레이(ms)로 변환

	for (int line = 0; tot_line > line; line++)
	{
		ticks_bigin = clock();

		point_add = 0;
		note_at_once = 0;

		for (int col = 0; 9 > col; col++)
			fscanf(note_ptr, "%1d", &load_data[col]);	// 채보 데이터를 한줄 로드

		// 채보파일 기반 노트 
		Clear_Note();	// 화면에 출력 되어있던 노트를 지움

		for (int col = 0; 9 > col; col++)
		{
			if (is_combo[col] == FALSE && load_data[col] != 0)
				Draw_Note(note_type, col, load_data[col]);	//	지정한 노트 타입으로 지정된 위치에 노트를 콘솔창에 출력
		}

		// 노트입력 및 입력한 버튼표시
		for (int i = 0; 2 > i; i++)
			note_input[i] = 0;	//	이전에 입력한 노트 데이터를 비움

		do
		{
			if (_kbhit() && note_at_once < 2)	// 키 입력이 있을경우, 이전에 입력한 키(key)가 1개 이하라면
			{
				int temp = _getch();	// 키 하나를 입력받음

				if (temp == 27)	// 입력한 키가 esc라면
				{
					while (_kbhit())
						_getch();

					Stop_Channel(sound_channel);	//	사운드채널 종료

					return goto_main;	//	메인화면으로 이동
				}
				else	//	esc키가 아니라면
					note_input[note_at_once] = temp - 48;	// 해당 키를 숫자로 변환

				if (note_input[note_at_once] >= 1 && note_input[note_at_once] <= 9)	// 입력한 키가 숫자키(1 ~ 9)라면
					note_at_once += 1;	//	입력한 키(key)를 +=1 연산
			}

			if (note_at_once >= 2)	//	입력한 키(key)가 2개 이상이라면 
				break;	//	입력을 중지하고 반복문 탈출

			ticks_mid = clock();
		} while ((ticks_mid - ticks_bigin) < delay_input);	//	입력한 키가 1개 이하이고, 시간이 총 delay의 절반만큼 지나지 않았다면 반복


		for (int i = 0; note_at_once > i; i++)
			Draw_Input(note_input[i]);	// 입력한 키(key)를 콘솔창에 출력

		// 점수계산 및 출력
		point_add = Check_Point(note_input, load_data, &combo, &result.combo_max, is_combo, &combo_level, result.hit);	// 이번 프레임에 점수와 콤보를 계산

		result.score += point_add * combo_level * COMBO_SCORE;	// 콤보 추가점수 비율을 곱하여 총점에 이번프레임에서 얻은 점수를 더함

		Move_Cursor(CURSOR_X_GAME - 1, CURSOR_Y_GAME - 2);

		printf("점수 : %4.0lf  콤보 : %3d 최대콤보 : %3d", result.score, combo, result.combo_max);	// 게임 진행상황 출력
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);
		
		Draw_Gauge(result.hit, tot_note);	// 게이지(옳게 입력한 키(key) / 총 노트수)를 출력합니다.

		// 싱크유지를 위한 딜레이
		do
		{
			ticks_last = clock();
		} while (ticks_last - ticks_bigin < delay_input * 2);
	}

	Clear_Note();	// 마지막 프레임에 출력된 노트, 키(key)를 지움

	Stop_Channel(sound_channel);	//	사운드채널 종료

	Sleep(1500);	// 결과창을 출력하기전 Delay

	final_choice = Show_Result(result, tot_note);	// 결과창을 출력하고 결과창에서 선택한 결과를 저장

	while (_kbhit())	//	입력버퍼를 비움
		_getch();

	return final_choice;	//	결과창에서 선택한 결과를 반환
}

void Hide_Cursor()	// 커서를 제거하여 콘솔창에 커서가 깜빡이는것을 방지
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Move_Cursor(int x, int y)	// 입력한 좌표(x, y)로 커서를 이동
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Draw_Note(int note_type, int posit, int note)	// 지정된 노트타입으로 출력할 위치로 이동한 뒤 노트를 출력
{
	int temp_x = posit % 3;
	int temp_y = posit / 3;

	int posit_x;
	int posit_y;

	posit_x = CURSOR_X_GAME + CURSOR_X_NOTE + temp_x * 8;
	posit_y = CURSOR_Y_GAME + CURSOR_Y_NOTE - temp_y * 4;

	Move_Cursor(posit_x, posit_y);

	Print_Note(note_type, note, posit_x, posit_y);

	return;
}

void Draw_Input(int posit)	// 사용자가 입력한 키(key)를 출력할 위치로 이동한 뒤 출력
{
	posit--;

	int temp_x = posit % 3;
	int temp_y = posit / 3;

	int posit_x;
	int posit_y;

	posit_x = CURSOR_X_GAME + CURSOR_X_NOTE + temp_x * 8;
	posit_y = CURSOR_Y_GAME + CURSOR_Y_NOTE - temp_y * 4;

	Move_Cursor(posit_x, posit_y);

	Print_Input(posit_x, posit_y);

	return;
}

void Print_Note(int note_type, int note, int posit_x, int posit_y)	// 해당 위치(x, y)로 이동하여 지정한 노트 타입으로 노트를 출력함
{
	Move_Cursor(posit_x, posit_y);

	if (note_type == 1)
	{
		switch (note)
		{
		case 1: printf("  □  "); break;
		case 2: printf("▒□▒"); break;
		case 3: printf("▒▒▒"); break;
		case 4: printf("▒▦▒"); break;
		case 5: printf("▦▦▦"); break;
		case 6: printf("▩▦▩"); break;
		case 7: printf("▩▩▩"); break;
		case 8: printf("▩■▩"); break;
		case 9: printf("■■■"); break;
		}

		Move_Cursor(posit_x, posit_y + 1);

		switch (note)
		{
		case 1: printf("□  □"); break;
		case 2: printf("□▒□"); break;
		case 3: printf("▒▒▒"); break;
		case 4: printf("▦▒▦"); break;
		case 5: printf("▦▦▦"); break;
		case 6: printf("▦▩▦"); break;
		case 7: printf("▩▩▩"); break;
		case 8: printf("■▩■"); break;
		case 9: printf("■■■"); break;
		}

		Move_Cursor(posit_x, posit_y + 2);

		switch (note)
		{
		case 1: printf("  □  "); break;
		case 2: printf("▒□▒"); break;
		case 3: printf("▒▒▒"); break;
		case 4: printf("▒▦▒"); break;
		case 5: printf("▦▦▦"); break;
		case 6: printf("▩▦▩"); break;
		case 7: printf("▩▩▩"); break;
		case 8: printf("▩■▩"); break;
		case 9: printf("■■■"); break;
		}
	}
	else if (note_type == 2)
	{
		switch (note)
		{
		case 1: printf("      "); break;
		case 2: printf("  □  "); break;
		case 3: printf("□□□"); break;
		case 4: printf("□▒□"); break;
		case 5: printf("▩▩▩"); break;
		case 6: printf("▦▦▦"); break;
		case 7: printf("▦■▦"); break;
		case 8: printf("■■■"); break;
		case 9: printf("■■■"); break;
		}

		Move_Cursor(posit_x, posit_y + 1);

		switch (note)
		{
		case 1: printf("  □  "); break;
		case 2: printf("□▒□"); break;
		case 3: printf("▒▒▒"); break;
		case 4: printf("□▩□"); break;
		case 5: printf("▩▦▩"); break;
		case 6: printf("▦■▦"); break;
		case 7: printf("■■■"); break;
		case 8: printf("■■■"); break;
		case 9: printf("■■■"); break;
		}

		Move_Cursor(posit_x, posit_y + 2);

		switch (note)
		{
		case 1: printf("      "); break;
		case 2: printf("  □  "); break;
		case 3: printf("□□□"); break;
		case 4: printf("□▒□"); break;
		case 5: printf("▩▩▩"); break;
		case 6: printf("▦▦▦"); break;
		case 7: printf("▦■▦"); break;
		case 8: printf("■■■"); break;
		case 9: printf("■■■"); break;
		}

	}
}

void Print_Input(int posit_x, int posit_y)	//	해당 위치로 이동하여 입력한 키(key)를 출력
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_Yellow);

	for (int i = 1; 3 >= i; i++)
	{
		printf("■■■");

		Move_Cursor(posit_x, posit_y + i);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);
}

int Check_Point(int note_input[], int note_ref[], int *combo, int* combo_max, bool is_combo[], int* combo_level, int* result_hit)	//	 점수와 콤보를 계산
{
	int score = 0;

	bool verify_nice = FALSE;
	bool verify_normal = FALSE;


	for (int i = 0; i < 2; i++)
	{
		if (note_input[i] > 0 && note_input[i] <= 9)
		{
			if (!is_combo[note_input[i] - 1] && note_ref[note_input[i] - 1])
			{
				is_combo[note_input[i] - 1] = TRUE;
				score += note_ref[note_input[i] - 1];

				if (note_ref[note_input[i] - 1] > 5)
				{
					verify_nice = TRUE;

					result_hit[nice] += 1;
				}
				else if (note_ref[note_input[i] - 1] > 0)
				{
					verify_normal = TRUE;

					result_hit[normal] += 1;
				}

				if (note_ref[note_input[i] - 1])
					(*combo)++;
			}
		}
	}

	if (verify_nice == TRUE)
	{
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[HIT_NICE], 0, &g_Channel[HIT_NICE]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bright_Green);
	}
	else if (verify_normal == TRUE)
	{
		FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[HIT_NORMAL], 0, &g_Channel[HIT_NORMAL]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_Green);
	}

	*combo_level = 1 + *combo / COMBO_MEASURE;

	if (*combo_max < *combo)
		*combo_max = *combo;

	for (int i = 0; i < 9; i++)
	{
		if (note_ref[i] == 9 && is_combo[i] == FALSE)
		{
			(*combo) *= 0;
			result_hit[miss] += 1;
		}
		if (note_ref[i] == 9 && is_combo[i] == TRUE)
			is_combo[i] = FALSE;
	}

	return score;
}

void Draw_Gauge(int* result_hit, int tot_note)	// 게이지(옳게 입력한 키(key) / 총 노트수)를 출력
{
	int gauge = GAUGE_MEASURE * (result_hit[nice] + result_hit[normal]) / tot_note;

	int gauge_x = CURSOR_X_GAME + CURSOR_X_GAGUE;
	int gauge_y = CURSOR_Y_GAME + CURSOR_Y_GAGUE;

	for (int i = 0; gauge > i; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 + gauge);

		Move_Cursor(gauge_x, gauge_y - i);

		printf("▒▒");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}

void Clear_Note()	//	그려진 노트와 키(key)를 지움
{
	for (int posit = 0; 9 > posit; posit++)
	{
		int temp_x = posit % 3;
		int temp_y = posit / 3;

		int posit_x;
		int posit_y;

		posit_x = CURSOR_X_GAME + CURSOR_X_NOTE + temp_x * 8;
		posit_y = CURSOR_Y_GAME + CURSOR_Y_NOTE - temp_y * 4;

		for (int i = 0; 3 > i; i++)
		{
			Move_Cursor(posit_x, posit_y + i);
			printf("      ");
		}
	}
}

int Show_Result(RESULT result, int tot_note)	//	게임 결과를 출력
{
	int	menu = 0;

	int	result_sound_channel = 0;

	system("cls");

	Print_Result_Frame();

	Print_Result(result, tot_note);

	Play_Sound(GAME_RESULT, &result_sound_channel);
	
	Select_Menu_Result(&menu);

	Stop_Channel(result_sound_channel);

	system("cls");

	switch (menu)
	{
	case play_more	: return play_more;	break;
	case goto_main	: return goto_main;	break;
	case exit_game	: return exit_game;	break;
	default			: return goto_main;	break;
	}
}

void Print_Result_Frame()	//	게임결과창에 배경, 프레임을 출력
{
	FILE* pTitle_result;

	unsigned char str[100];

	int i = 0;

	pTitle_result = fopen("../source/title/title_result.txt", "r");

	while (feof(pTitle_result) == 0)
	{
		Move_Cursor(CURSOR_X_TITLE_RESULT, CURSOR_Y_TITLE_RESULT + i);

		fgets(str, 100, pTitle_result);
		printf("%s", str);

		i++;
	}

	Move_Cursor(0, CURSOR_Y_RESULT - 2);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");

	for (int i = 0; CURSOR_Y_RESULT_SIZE > i; i++)
	{
		Move_Cursor(CURSOR_X_RESULT, CURSOR_Y_RESULT + i);
		printf("|");
	}

	Move_Cursor(0, CURSOR_Y_SELECT_RESULT - 2);
	for (int i = 0; WINDOWSIZE_X > i; i++)
		printf("=");
}

void Print_Result(RESULT result, int tot_note)	//	 게임 결과창에 결과를 출력
{
	result.note_percent = PERCENT_MEASURE * (result.hit[nice] + result.hit[normal]) / (double)tot_note;
	result.combo_percent = result.combo_max / (double)tot_note;

	Move_Cursor(CURSOR_X_RESULT - CURSOR_X_RESULT_SIZE01, CURSOR_Y_RESULT);
	printf("최종 점수 : %.0lf", result.score);

	Move_Cursor(CURSOR_X_RESULT - CURSOR_X_RESULT_SIZE01, CURSOR_Y_RESULT + 2);
	if (result.combo_percent >= 75)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bright_Sky);
	else if (result.combo_percent == 100)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bright_Green);
	printf("MAX COMBO : %d", result.combo_max);

	Move_Cursor(CURSOR_X_RESULT - CURSOR_X_RESULT_SIZE01, CURSOR_Y_RESULT + 4);
	if (result.note_percent >= 50)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bright_Sky);
	else if (result.note_percent >= 90)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bright_Green);
	printf("성공률 : %.2lf%%", result.note_percent);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT);
	printf("Nice : %d", result.hit[nice]);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT + 2);
	printf("Normal : %d", result.hit[normal]);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT + 4);
	printf("Miss : %d", result.hit[miss]);

}

void Select_Menu_Result(int* menu)	//	게임 결과창에 선택창을 출력
{
	int	is_end = 0;
	int	input = 0;

	Move_Cursor(CURSOR_X_SELECT_RESULT + 3, CURSOR_Y_SELECT_RESULT);
	printf("무엇을 할까요?");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2);
	printf("▶1. 한번더 플레이!");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 3);
	printf("  2. 메인 메뉴로 이동");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 4);
	printf("  3. 그만할래요");

	do
	{
		is_end = 0;
		input = 0;

		if (_kbhit())
		{
			is_end = _getch();

			if (is_end != MENU_END)
				input = _getch();

			switch (*menu)
			{
			case 0: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("  "); break;
			case 1: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("  "); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("  "); break;
			}

			if (input == 72)
			{
				if (*menu > 0)
					*menu -= 1;
			}
			else if (input == 80)
			{
				if (*menu < 2)
					*menu += 1;
			}
			else
				continue;

			switch (*menu)
			{
			case 0: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("▶"); break;
			case 1: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("▶"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("▶"); break;
			}
		}


	} while (is_end != MENU_END);

	if (_kbhit())
		_getch();
}