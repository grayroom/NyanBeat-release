#include "pch.h"

#include "iohandler.h"
#include "common.h"

std::mutex mUsrKey;

void NyanIO::Input::listenKeyStat(const int option) {
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		mUsrKey.lock();
		keyStat.numKey = 0;
		keyStat.cmdKey = 0;

		for (int i = 0; i < gMode; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & option) {
				keyStat.numKey |= 0b1 << i;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & option) {
			keyStat.cmdKey = VK_ESCAPE;
		}
		mUsrKey.unlock();
	} while (true);
}

NyanIO::Keyset NyanIO::Input::getKeyStat() {
	return keyStat;
}

NyanIO::Output::Output() {
	// ...
}

NyanIO::Output::Output(NyanIO::Keyset& keyStat) {
	usrNumKey = &keyStat.numKey; // get Input classes' Keystat.numKey
	usrCmdKey = &keyStat.cmdKey; // get Input classes' Keystat.cmdKey

	hideCursor();

	hThread = new std::vector<std::thread>{ gMode };
	for (int i = 0; i < gMode; ++i) {
		hThread->emplace_back(&drawKey, this, i);
	}
}

void NyanIO::Output::listenKeyStat() {
	std::thread hSysKeyThread{ &listenSysKeyStat, this };
	std::thread hUsrKeyThread{ &listenUsrKeyStat, this };
	std::thread hClock{ &listenClock, this };

	hSysKeyThread.join();
	hUsrKeyThread.join();
}

void NyanIO::Output::listenSysKeyStat() {
	// ...
}

void NyanIO::Output::listenUsrKeyStat() {
	for (int i = 0; i < gMode; ++i) {
		if ((*usrNumKey | 0b000000001 << i) == (0b000000001 << i)) {

			cvUsrKey.notify_all();
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void NyanIO::Output::listenClock(const int period) {
	do {
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		do { // wait for period(ms)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		} while (std::chrono::system_clock::now() - start == std::chrono::milliseconds(period));

		mGlobTick.lock();
		tick = gMode;
		mGlobTick.unlock();

		cvClock.notify_all();
	} while (!isTerminated);
}

void NyanIO::Output::drawKey(const int numKey) {
	int posX{ numKey % 3 + 1 }, posY{ numKey / 3 + 1 };
	int currPhase{ gMode };
	std::chrono::system_clock::time_point start;

	while (!isTerminated) {
		start = std::chrono::system_clock::now();

		std::unique_lock<std::mutex> mTick(mGlobTick);	// Sync to clock
		cvClock.wait(mTick, [&] { return tick > 0; });
		tick--;
		mTick.unlock();
		
		std::unique_lock<std::mutex> mUsrNumKey(mUsrKey);		// Sync to user key
		cvUsrKey.wait(mUsrNumKey, [&] { return (*usrNumKey | 0b000000001 << numKey) == (0b000000001 << numKey); });
		*usrNumKey &= ~(0b000000001 << numKey);
		mUsrNumKey.unlock();
		
		currPhase--;
		moveCursor(posX, posY);
		std::cout << currPhase;

		std::this_thread::sleep_until(start + std::chrono::milliseconds(1)); // loop period is 1ms
	}
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