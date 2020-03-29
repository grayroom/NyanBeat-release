#include "pch.h"

#include "iohandler.h"
#include "common.h"

NyanIO::Input::Input() {
	// ...
}

NyanIO::Input::Input(NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode) {

}

void NyanIO::Input::listenKeyStat(const int option) {
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		mUsrKey->lock();
		keyStat->numKey = 0;
		keyStat->cmdKey = 0;

		for (int i = 0; i < gMode; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & option) {
				keyStat->numKey |= 0b1 << i;
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & option) {
			keyStat->cmdKey = VK_ESCAPE;
		}
		mUsrKey->unlock();
	} while (true);
}

NyanIO::Keyset NyanIO::Input::getKeyStat() {
	return *keyStat;
}

NyanIO::Output::Output()
	:keyStat{ nullptr }, mUsrKey{ nullptr }, gMode{ 0 } {}

NyanIO::Output::Output(NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode)
	:keyStat{ keyStat }, mUsrKey { pMutex }, gMode{ gMode } {
	cvUsrKey = new std::condition_variable[gMode];
	hKeyThread = new std::thread*[gMode];

	hideCursor();
}

void NyanIO::Output::outputFrame() {
	for (int i = 0; i < gMode; ++i) {
		hKeyThread[i] = new std::thread{ &drawKey, this, i };
	}
	
	std::thread hSysKeyThread{ &listenSysKeyStat, this };
	std::thread hUsrKeyThread{ &listenUsrKeyStat, this };
	std::thread hClock{ &listenClock, this };

	hSysKeyThread.join();
	hUsrKeyThread.join();
	hClock.join();
}

void NyanIO::Output::listenSysKeyStat() {
	// ...
}

void NyanIO::Output::listenUsrKeyStat() {
	for (int i = 0; i < gMode; ++i) {
		if ((keyStat->numKey | 0b000000001 << i) == (0b000000001 << i)) {

			cvUsrKey[i].notify_one();
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
		std::unique_lock<std::mutex> mUsrNumKey(*mUsrKey);	// Sync to user key
		cvUsrKey[numKey].wait(mUsrNumKey, [&] { return (keyStat->numKey | 0b000000001 << numKey) == (0b000000001 << numKey); });
		keyStat->numKey &= ~(0b000000001 << numKey);
		mUsrNumKey.unlock();

		while ((keyStat->numKey | 0b000000001 << numKey) != (0b000000001 << numKey)) { // while no additional key
			std::unique_lock<std::mutex> mTick(mGlobTick);		// Sync to clock
			cvClock.wait(mTick, [&] { return tick > 0; });
			start = std::chrono::system_clock::now();
			tick--;
			mTick.unlock();

			currPhase--;
			moveCursor(posX, posY);
			std::cout << currPhase;

			std::this_thread::sleep_until(start + std::chrono::milliseconds(1)); // loop period is 1ms
		}
	}
}


void NyanIO::initNyanIO(Input* pInput, Output* pOutput, NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode) {
	keyStat = new NyanIO::Keyset{};
	pMutex = new std::mutex;

	pInput = new NyanIO::Input{ keyStat, pMutex, gMode };
	pOutput = new NyanIO::Output{ keyStat, pMutex, gMode };
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