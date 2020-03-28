#include "pch.h"
#include "iohandler.h"

HANDLE hMutex_keyStat;

NyanIO::Keyset NyanIO::Input::keyStat;

unsigned __stdcall NyanIO::Input::listenKeyStat(void* arg) {
	int option{ *(int*)arg };

	do {
		Sleep(1);
		WaitForSingleObject(hMutex_keyStat, INFINITE); // critial section control for argument(keyPress) 
		keyStat.numKey = 0;
		keyStat.cmdKey = 0;

		for (int i = 0; i < 9; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & option) {
				keyStat.numKey |= 0b1 << i;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & option) {
			keyStat.cmdKey = VK_ESCAPE;
		}
		ReleaseMutex(hMutex_keyStat); // end
	} while (true);

	return 0;
}

NyanIO::Keyset NyanIO::Input::getKeyStat() {
	return keyStat;
}

NyanIO::Output::Output()
	: keyPos{ {1,3}, {2,3}, {3,3}, {1,2}, {2,2}, {3,2}, {1,1}, {2,1}, {3,1} } {}

NyanIO::Output::Output(NyanIO::Keyset& keyStat)
	: keyPos{ {1,3}, {2,3}, {3,3}, {1,2}, {2,2}, {3,2}, {1,1}, {2,1}, {3,1} } {
	usrKey = &keyStat.numKey;
	cmdKey = &keyStat.cmdKey;
}

void NyanIO::Output::drawSysKey() {
	for (int i = 0; i < 9; ++i) {
		if ((*usrKey & 0b000000001 << i) == (0b000000001 << i)) {
			TerminateThread(hKeyThread[i], 0);
			hKeyThread[i] = (HANDLE)_beginthreadex(NULL, 0, NyanIO::Output::DrawAnimatedKey, (void*)keyPos[i], 0, &keyTid[i]);
		}
	}
}

unsigned __stdcall NyanIO::Output::DrawAnimatedKey(void* arg) {
	int posX{ ((int*)arg)[0] },
		posY{ ((int*)arg)[1] };

	for (int i = 0; i < 9; ++i) {
		moveCursor(posX, posY);
		std::cout << i;
		Sleep(500);
	}

	return 0;
}

void NyanIO::hideCursor() {
	CONSOLE_CURSOR_INFO cursor{};
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void NyanIO::moveCursor(const int x, const int y) {
	COORD cursor;
	cursor.X = x;
	cursor.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}