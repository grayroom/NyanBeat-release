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

	Init_Sound();	//	sound포인터에 각각의 사운드를 연결

	Hide_Cursor();	// 커서를 눈에 띄지 않게 숨김
	
	do
	{
		system("cls");

		Play_Sound(MAIN, &main_sound_channel);	// 메인메뉴 배경음 재생

		selected_menu = Menu();	// 메인메뉴를 선택

		Stop_Channel(main_sound_channel);	// 메인메뉴 배경음 종료

		switch (selected_menu)	// 메인메뉴에서 선택한 메뉴 실행
		{
		case 1: Play_Game(note_ptr, background_type, note_type, &final_choice);	break;
		case 2: Setup(&background_type, &note_type);							break;
		case 3: break;
		}

		if (selected_menu == 1)	// 게임을 실행한 뒤
		{
			switch (final_choice)	// final_choice가
			{
			case goto_main: selected_menu = 0; break;	// goto_main인 경우 선택한 메뉴를 0으로 초기화
			case exit_game: selected_menu = 3; break;	// exit_game인 경우 선택한 메뉴를 3으로 변경
			}
		}
	} while (selected_menu != 3);	// 선택한 메뉴가 3이라면 탈출

	Release_ALL_Sound();	// 모든 사운드를 반환


	return 0;	// 프로그램 종료
}