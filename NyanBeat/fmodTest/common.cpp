#include "common.h"

extern HANDLE hMutex;

unsigned __stdcall listenKeyPress(void* arg) {
	KeySet* keyPress{(KeySet*)arg};

	do {
		Sleep(1);
		WaitForSingleObject(hMutex, INFINITE);
		keyPress->numKey = 0;
		keyPress->cmdKey = 0;

		for (int i = 0; i < 9; i++) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & 0x0001) {
				keyPress->numKey = keyPress->numKey | 0b1 << i;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) {
			keyPress->cmdKey = VK_ESCAPE;
		}
		ReleaseMutex(hMutex);
	} while (true);

	return 0;
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursor{};
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void moveCursor(const int x, const int y) {
	COORD cursor;
	cursor.X = x;
	cursor.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

void errCheck(const FMOD_RESULT result) {
	if (result != FMOD_OK) {
		errPrint("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void errPrint(const char* format, ...) {
	char errMsg[ERR_MSG_LEN];

	va_list args;
	va_start(args, format);
	vsnprintf(errMsg, ERR_MSG_LEN, format, args);
	//TODO: 아래 코드를 삭제하고, 콘솔을 일관되게 업데이트 할 수 있는 함수를 구현할 것
	perror(errMsg);
	va_end(args);
	//errMsg[ERR_MSG_LEN - 1] = '\0';

	exit(0);
}