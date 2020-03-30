#include "pch.h"

#include "iohandler.h"
#include "common.h"


NyanIO::Input::Input()
	:keyUsrStat{ nullptr }, mUsrKey{ nullptr }, gMode{} {}

NyanIO::Input::Input(Keyset* keyStat, mutex* pMutex, vector<conVar*>* cvs, const int gMode)
	:keyUsrStat{ keyStat }, mUsrKey{ pMutex }, gMode{ gMode } {
	int i{};

	cvUsrKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvUsrKey[j] = cvs->at(i);
	}

	cvCmdKey = cvs->at(i++);

	cvSysKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvSysKey[j] = cvs->at(i);
	}

	cvClock = cvs->at(i);
}

void NyanIO::Input::inputFrame() {
	tKeyListen = new thread{ &listenKeyStat, this };

	tKeyListen->join();
}

void NyanIO::Input::listenKeyStat(const int option) {
	do {
		this_thread::sleep_for(chrono::milliseconds(1));
		mUsrKey->lock();
		keyUsrStat->numKey = 0;
		keyUsrStat->cmdKey = 0;
		mUsrKey->unlock();

		for (int i = 0; i < gMode; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & option) {
				mUsrKey->lock();
				keyUsrStat->numKey |= 0b1 << i;
				mUsrKey->unlock();
				cvUsrKey[i]->notify_one();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & option) {
			mUsrKey->lock();
			keyUsrStat->cmdKey = VK_ESCAPE;
			mUsrKey->unlock();
			cvCmdKey->notify_all();
		}
		mUsrKey->unlock();
	} while (true); //TODO: 종료조건 정할것
}

NyanIO::Keyset NyanIO::Input::getKeyStat() {
	return *keyUsrStat;
}


NyanIO::Output::Output()
	:keyUsrStat{ nullptr }, mUsrKey{ nullptr }, gMode{ 0 } {}

NyanIO::Output::Output(Keyset* keyStat, mutex* pMutex, vector<conVar*>* cvs, const int gMode)
	:keyUsrStat{ keyStat }, mUsrKey { pMutex }, gMode{ gMode } {
	tKeyDraw = new thread*[gMode];
	
	int i{};

	cvUsrKey = new conVar*[gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvUsrKey[j] = cvs->at(i);
	}

	cvCmdKey = cvs->at(i++);

	cvSysKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvSysKey[j] = cvs->at(i);
	}

	cvClock = cvs->at(i);

	hideCursor();
}

void NyanIO::Output::outputFrame() {
	for (int i = 0; i < gMode; ++i) {
		hThreads.push_back(tKeyDraw[i] = new thread{ &drawKey, this, i });
	}

	//hThreads.push_back(hUsrThread = new thread{ &listenUsrKeyStat, this });
	//hThreads.push_back(hSysThread = new thread{ &listenSysKeyStat, this });
	hThreads.push_back(tClock = new thread{ &listenClock, this });

	for (thread* xThread : hThreads) {
		xThread->join();
	}
}

void NyanIO::Output::listenClock(const int period) { //TDOO: 클록을 모든 개체가 공유하도록 변경해야함
	do {
		chrono::system_clock::time_point start = chrono::system_clock::now();
		do { // wait for period(ms)
			this_thread::sleep_for(chrono::milliseconds(1));
		} while (chrono::system_clock::now() - start == chrono::milliseconds(period));

		mGlobTick->lock();
		tick = gMode;
		mGlobTick->unlock();

		cvClock->notify_all();
	} while (!isTerminated);
}

void NyanIO::Output::drawKey(const int numKey) {
	int posX{ numKey % 3 + 1 }, posY{ numKey / 3 + 1 };
	int currPhase{ gMode };
	chrono::system_clock::time_point start;

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsrKey);	// Sync to user key
		cvUsrKey[numKey]->wait(mUsrNumKey, [&] { return (keyUsrStat->numKey | 0b1 << numKey) == (0b1 << numKey); });
		keyUsrStat->numKey &= ~(0b1 << numKey);
		mUsrNumKey.unlock();

		while ((keyUsrStat->numKey | 0b1 << numKey) != (0b1 << numKey)) { // while no additional key
			unique_lock<mutex> mTick(*mGlobTick);		// Sync to clock
			cvClock->wait(mTick, [&] { return tick > 0; });
			start = chrono::system_clock::now();
			tick--;
			mTick.unlock();

			currPhase--;
			moveCursor(posX, posY);
			cout << currPhase;

			this_thread::sleep_until(start + chrono::milliseconds(1)); // loop period is 1ms
		}
	}
}


void NyanIO::initNyanIO(Input* pInput, Output* pOutput, Keyset* keyStat, mutex* pMutex, vector<conVar*>* cvs, const int gMode) {
	keyStat = new Keyset{};
	pMutex = new mutex{};
	for (int i = 0; i < (2 * gMode + 2); ++i) {
		cvs->push_back( new conVar{} );
	}

	pInput = new Input{ keyStat, pMutex, cvs, gMode };
	pOutput = new Output{ keyStat, pMutex, cvs, gMode };
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