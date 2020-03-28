
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


	// ��Ÿ������ �ε�
	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_line[col]);
	fscanf(note_ptr, "%1c", &trashbin);	// %1c�� �Է¹������ WhiteSpace�� �ɷ��� �� �����Ƿ� ���

	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_rate[col]);
	fscanf(note_ptr, "%1c", &trashbin);

	for (int col = 0; 9 > col; col++)
		fscanf(note_ptr, "%1c", &metadata_note[col]);
	fscanf(note_ptr, "%1c", &trashbin);

	tot_line = atoi(metadata_line);
	frame_rate = atoi(metadata_rate);
	tot_note = atoi(metadata_note);

	delay_input = CLOCKS_PER_SEC / (2 * frame_rate);	// ��Ÿ�����Ϳ��� �ε��� �����ӷ�(fps)�� ������(ms)�� ��ȯ

	for (int line = 0; tot_line > line; line++)
	{
		ticks_bigin = clock();

		point_add = 0;
		note_at_once = 0;

		for (int col = 0; 9 > col; col++)
			fscanf(note_ptr, "%1d", &load_data[col]);	// ä�� �����͸� ���� �ε�

		// ä������ ��� ��Ʈ 
		Clear_Note();	// ȭ�鿡 ��� �Ǿ��ִ� ��Ʈ�� ����

		for (int col = 0; 9 > col; col++)
		{
			if (is_combo[col] == FALSE && load_data[col] != 0)
				Draw_Note(note_type, col, load_data[col]);	//	������ ��Ʈ Ÿ������ ������ ��ġ�� ��Ʈ�� �ܼ�â�� ���
		}

		// ��Ʈ�Է� �� �Է��� ��ưǥ��
		for (int i = 0; 2 > i; i++)
			note_input[i] = 0;	//	������ �Է��� ��Ʈ �����͸� ���

		do
		{
			if (_kbhit() && note_at_once < 2)	// Ű �Է��� �������, ������ �Է��� Ű(key)�� 1�� ���϶��
			{
				int temp = _getch();	// Ű �ϳ��� �Է¹���

				if (temp == 27)	// �Է��� Ű�� esc���
				{
					while (_kbhit())
						_getch();

					Stop_Channel(sound_channel);	//	����ä�� ����

					return goto_main;	//	����ȭ������ �̵�
				}
				else	//	escŰ�� �ƴ϶��
					note_input[note_at_once] = temp - 48;	// �ش� Ű�� ���ڷ� ��ȯ

				if (note_input[note_at_once] >= 1 && note_input[note_at_once] <= 9)	// �Է��� Ű�� ����Ű(1 ~ 9)���
					note_at_once += 1;	//	�Է��� Ű(key)�� +=1 ����
			}

			if (note_at_once >= 2)	//	�Է��� Ű(key)�� 2�� �̻��̶�� 
				break;	//	�Է��� �����ϰ� �ݺ��� Ż��

			ticks_mid = clock();
		} while ((ticks_mid - ticks_bigin) < delay_input);	//	�Է��� Ű�� 1�� �����̰�, �ð��� �� delay�� ���ݸ�ŭ ������ �ʾҴٸ� �ݺ�


		for (int i = 0; note_at_once > i; i++)
			Draw_Input(note_input[i]);	// �Է��� Ű(key)�� �ܼ�â�� ���

		// ������� �� ���
		point_add = Check_Point(note_input, load_data, &combo, &result.combo_max, is_combo, &combo_level, result.hit);	// �̹� �����ӿ� ������ �޺��� ���

		result.score += point_add * combo_level * COMBO_SCORE;	// �޺� �߰����� ������ ���Ͽ� ������ �̹������ӿ��� ���� ������ ����

		Move_Cursor(CURSOR_X_GAME - 1, CURSOR_Y_GAME - 2);

		printf("���� : %4.0lf  �޺� : %3d �ִ��޺� : %3d", result.score, combo, result.combo_max);	// ���� �����Ȳ ���
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);
		
		Draw_Gauge(result.hit, tot_note);	// ������(�ǰ� �Է��� Ű(key) / �� ��Ʈ��)�� ����մϴ�.

		// ��ũ������ ���� ������
		do
		{
			ticks_last = clock();
		} while (ticks_last - ticks_bigin < delay_input * 2);
	}

	Clear_Note();	// ������ �����ӿ� ��µ� ��Ʈ, Ű(key)�� ����

	Stop_Channel(sound_channel);	//	����ä�� ����

	Sleep(1500);	// ���â�� ����ϱ��� Delay

	final_choice = Show_Result(result, tot_note);	// ���â�� ����ϰ� ���â���� ������ ����� ����

	while (_kbhit())	//	�Է¹��۸� ���
		_getch();

	return final_choice;	//	���â���� ������ ����� ��ȯ
}

void Hide_Cursor()	// Ŀ���� �����Ͽ� �ܼ�â�� Ŀ���� �����̴°��� ����
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Move_Cursor(int x, int y)	// �Է��� ��ǥ(x, y)�� Ŀ���� �̵�
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Draw_Note(int note_type, int posit, int note)	// ������ ��ƮŸ������ ����� ��ġ�� �̵��� �� ��Ʈ�� ���
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

void Draw_Input(int posit)	// ����ڰ� �Է��� Ű(key)�� ����� ��ġ�� �̵��� �� ���
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

void Print_Note(int note_type, int note, int posit_x, int posit_y)	// �ش� ��ġ(x, y)�� �̵��Ͽ� ������ ��Ʈ Ÿ������ ��Ʈ�� �����
{
	Move_Cursor(posit_x, posit_y);

	if (note_type == 1)
	{
		switch (note)
		{
		case 1: printf("  ��  "); break;
		case 2: printf("�ơ��"); break;
		case 3: printf("�ƢƢ�"); break;
		case 4: printf("�Ƣˢ�"); break;
		case 5: printf("�ˢˢ�"); break;
		case 6: printf("�̢ˢ�"); break;
		case 7: printf("�̢̢�"); break;
		case 8: printf("�̡��"); break;
		case 9: printf("����"); break;
		}

		Move_Cursor(posit_x, posit_y + 1);

		switch (note)
		{
		case 1: printf("��  ��"); break;
		case 2: printf("��ơ�"); break;
		case 3: printf("�ƢƢ�"); break;
		case 4: printf("�ˢƢ�"); break;
		case 5: printf("�ˢˢ�"); break;
		case 6: printf("�ˢ̢�"); break;
		case 7: printf("�̢̢�"); break;
		case 8: printf("��̡�"); break;
		case 9: printf("����"); break;
		}

		Move_Cursor(posit_x, posit_y + 2);

		switch (note)
		{
		case 1: printf("  ��  "); break;
		case 2: printf("�ơ��"); break;
		case 3: printf("�ƢƢ�"); break;
		case 4: printf("�Ƣˢ�"); break;
		case 5: printf("�ˢˢ�"); break;
		case 6: printf("�̢ˢ�"); break;
		case 7: printf("�̢̢�"); break;
		case 8: printf("�̡��"); break;
		case 9: printf("����"); break;
		}
	}
	else if (note_type == 2)
	{
		switch (note)
		{
		case 1: printf("      "); break;
		case 2: printf("  ��  "); break;
		case 3: printf("����"); break;
		case 4: printf("��ơ�"); break;
		case 5: printf("�̢̢�"); break;
		case 6: printf("�ˢˢ�"); break;
		case 7: printf("�ˡ��"); break;
		case 8: printf("����"); break;
		case 9: printf("����"); break;
		}

		Move_Cursor(posit_x, posit_y + 1);

		switch (note)
		{
		case 1: printf("  ��  "); break;
		case 2: printf("��ơ�"); break;
		case 3: printf("�ƢƢ�"); break;
		case 4: printf("��̡�"); break;
		case 5: printf("�̢ˢ�"); break;
		case 6: printf("�ˡ��"); break;
		case 7: printf("����"); break;
		case 8: printf("����"); break;
		case 9: printf("����"); break;
		}

		Move_Cursor(posit_x, posit_y + 2);

		switch (note)
		{
		case 1: printf("      "); break;
		case 2: printf("  ��  "); break;
		case 3: printf("����"); break;
		case 4: printf("��ơ�"); break;
		case 5: printf("�̢̢�"); break;
		case 6: printf("�ˢˢ�"); break;
		case 7: printf("�ˡ��"); break;
		case 8: printf("����"); break;
		case 9: printf("����"); break;
		}

	}
}

void Print_Input(int posit_x, int posit_y)	//	�ش� ��ġ�� �̵��Ͽ� �Է��� Ű(key)�� ���
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_Yellow);

	for (int i = 1; 3 >= i; i++)
	{
		printf("����");

		Move_Cursor(posit_x, posit_y + i);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);
}

int Check_Point(int note_input[], int note_ref[], int *combo, int* combo_max, bool is_combo[], int* combo_level, int* result_hit)	//	 ������ �޺��� ���
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

void Draw_Gauge(int* result_hit, int tot_note)	// ������(�ǰ� �Է��� Ű(key) / �� ��Ʈ��)�� ���
{
	int gauge = GAUGE_MEASURE * (result_hit[nice] + result_hit[normal]) / tot_note;

	int gauge_x = CURSOR_X_GAME + CURSOR_X_GAGUE;
	int gauge_y = CURSOR_Y_GAME + CURSOR_Y_GAGUE;

	for (int i = 0; gauge > i; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 + gauge);

		Move_Cursor(gauge_x, gauge_y - i);

		printf("�Ƣ�");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}

void Clear_Note()	//	�׷��� ��Ʈ�� Ű(key)�� ����
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

int Show_Result(RESULT result, int tot_note)	//	���� ����� ���
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

void Print_Result_Frame()	//	���Ӱ��â�� ���, �������� ���
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

void Print_Result(RESULT result, int tot_note)	//	 ���� ���â�� ����� ���
{
	result.note_percent = PERCENT_MEASURE * (result.hit[nice] + result.hit[normal]) / (double)tot_note;
	result.combo_percent = result.combo_max / (double)tot_note;

	Move_Cursor(CURSOR_X_RESULT - CURSOR_X_RESULT_SIZE01, CURSOR_Y_RESULT);
	printf("���� ���� : %.0lf", result.score);

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
	printf("������ : %.2lf%%", result.note_percent);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Dark_White);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT);
	printf("Nice : %d", result.hit[nice]);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT + 2);
	printf("Normal : %d", result.hit[normal]);

	Move_Cursor(CURSOR_X_RESULT + CURSOR_X_RESULT_SIZE02, CURSOR_Y_RESULT + 4);
	printf("Miss : %d", result.hit[miss]);

}

void Select_Menu_Result(int* menu)	//	���� ���â�� ����â�� ���
{
	int	is_end = 0;
	int	input = 0;

	Move_Cursor(CURSOR_X_SELECT_RESULT + 3, CURSOR_Y_SELECT_RESULT);
	printf("������ �ұ��?");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2);
	printf("��1. �ѹ��� �÷���!");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 3);
	printf("  2. ���� �޴��� �̵�");
	Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 4);
	printf("  3. �׸��ҷ���");

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
			case 0: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("��"); break;
			case 1: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("��"); break;
			case 2: Move_Cursor(CURSOR_X_SELECT_RESULT, CURSOR_Y_SELECT_RESULT + 2 + *menu); printf("��"); break;
			}
		}


	} while (is_end != MENU_END);

	if (_kbhit())
		_getch();
}