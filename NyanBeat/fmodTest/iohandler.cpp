#include "pch.h"

#include "iohandler.h"
#include "common.h"


NyanIO::Input::Input()
	:keyUsrStat{ nullptr }, mUsrKey{ nullptr }, gMode{} {}

NyanIO::Input::Input(Keyset*& keyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode)
	: keyUsrStat{ keyStat }, gMode{ gMode }, isTerminated{ false } {
	mUsrKey = hMutex[0];
	mSysKey = hMutex[1];

	int i{};
	cvUsrKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvUsrKey[j] = cvs.at(i);
	}

	cvCmdKey = cvs.at(i++);

	cvSysKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvSysKey[j] = cvs.at(i);
	}

	cvClock = cvs.at(i);
}

void NyanIO::Input::inputFrame(const int period) {
	tKeyListen = new thread{ &NyanIO::Input::listenKeyStat, this, NYANIO_PAST };
	tClock = new thread{ &NyanIO::Input::listenClock, this, period };

	tKeyListen->join();
	tClock->join();
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
	} while (true); //TODO: 종료조건 정할것
}

void NyanIO::Input::listenClock(const int period) { //TDOO: 클록을 모든 개체가 공유하도록
	do {
		chrono::system_clock::time_point start = chrono::system_clock::now();
		do { // wait for period(ms)
			this_thread::sleep_for(chrono::milliseconds(1));
		} while (chrono::system_clock::now() - start < chrono::milliseconds(period));

		cvClock->notify_all();
	} while (!isTerminated);
}

NyanIO::Keyset* NyanIO::Input::getKeyStat() {
	return keyUsrStat;
}


NyanIO::Output::Output()
	:keyUsrStat{ nullptr }, mUsrKey{ nullptr }, gMode{ 0 } {}

NyanIO::Output::Output(Keyset*& keyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode)
	: keyUsrStat{ keyStat }, gMode{ gMode }, isTerminated{ false } {
	mUsrKey = hMutex[0];
	mSysKey = hMutex[1];
	mKeyDraw = new mutex{};
	
	int i{};

	cvUsrKey = new conVar*[gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvUsrKey[j] = cvs.at(i);
	}

	cvCmdKey = cvs.at(i++);

	cvSysKey = new conVar * [gMode];
	for (int j = 0; j < gMode; ++i, ++j) {
		cvSysKey[j] = cvs.at(i);
	}

	cvClock = cvs.at(i);

	hideCursor();
}

void NyanIO::Output::outputFrame() {
	tKeyDraw = new thread * [gMode];
	for (int i = 0; i < gMode; ++i) {
		tKeyDraw[i] = new thread{ &NyanIO::Output::drawKey, this, i };
		hThreads.push_back(tKeyDraw[i]);
	}

	for (thread* xThread : hThreads) {
		xThread->join();
	}
}

void NyanIO::Output::drawKey(const int numKey) {
	int posX{ numKey % 3 + 1 }, posY{ numKey / 3 + 1 };
	int currPhase{};
	chrono::system_clock::time_point start;

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsrKey);	// Sync to user key
		cvUsrKey[numKey]->wait(mUsrNumKey, [&] { return (keyUsrStat->numKey & (0b1 << numKey)) == (0b1 << numKey); });
		keyUsrStat->numKey &= ~(0b1 << numKey);		// resolve key-on status
		mUsrNumKey.unlock();						// check

		currPhase = gMode;

		while ((keyUsrStat->numKey & (0b1 << numKey)) != (0b1 << numKey)) {	// while no additional key
			unique_lock<mutex> mTick(*mKeyDraw);	// Sync to clock
			cvClock->wait(mTick, [&] { return tick > 0; }); //TODO: 여기조건부터 수정시작!
			start = chrono::system_clock::now();
			mTick.unlock();	// check

			currPhase--;	// draw current phase
			moveCursor(posX, posY);
			cout << currPhase;

			this_thread::sleep_until(start + chrono::milliseconds(1)); // loop period is 1ms
		}
	}
}


void NyanIO::initNyanIO(Input*& pInput, Output*& pOutput, Keyset*& pKeyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode) {
	pKeyStat = new Keyset{};

	mutex* mTemp{};
	for (int i = 0; i < 2; ++i) {
		mTemp = new mutex;
		hMutex.push_back(mTemp);
	}

	conVar* mConVar{};
	for (int i = 0; i < (2 * gMode + 2); ++i) {
		mConVar = new conVar;
		cvs.push_back(mConVar);
	}

	pInput = new Input{ pKeyStat, hMutex, cvs, gMode };
	pOutput = new Output{ pKeyStat, hMutex, cvs, gMode };
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