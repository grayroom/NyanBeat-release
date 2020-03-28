#include "game_general.h"
#include "game_sound.h"

int main(void)
{
	FILE*	note_ptr = NULL;

	int		selected_menu = 0;
	int		background_type = 1;
	int		note_type = 1;

	int		main_sound_channel = 0;

	int		final_choice = 0;


	system("title Nyan Beat");
	system("mode con cols=83 lines=35");

	Init_Sound();	//	sound�����Ϳ� ������ ���带 ����

	Hide_Cursor();	// Ŀ���� ���� ���� �ʰ� ����
	
	do
	{
		system("cls");

		Play_Sound(MAIN, &main_sound_channel);	// ���θ޴� ����� ���

		selected_menu = Menu();	// ���θ޴��� ����

		Stop_Channel(main_sound_channel);	// ���θ޴� ����� ����

		switch (selected_menu)	// ���θ޴����� ������ �޴� ����
		{
		case 1: Play_Game(note_ptr, background_type, note_type, &final_choice);	break;
		case 2: Setup(&background_type, &note_type);							break;
		case 3: break;
		}

		if (selected_menu == 1)	// ������ ������ ��
		{
			switch (final_choice)	// final_choice��
			{
			case goto_main: selected_menu = 0; break;	// goto_main�� ��� ������ �޴��� 0���� �ʱ�ȭ
			case exit_game: selected_menu = 3; break;	// exit_game�� ��� ������ �޴��� 3���� ����
			}
		}
	} while (selected_menu != 3);	// ������ �޴��� 3�̶�� Ż��

	Release_ALL_Sound();	// ��� ���带 ��ȯ


	return 0;	// ���α׷� ����
}