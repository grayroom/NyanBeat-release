#include "pch.h"

#include "iohandler.h"
#include "common.h"


Nyan::Input::Input()
	:usrKeyStat{ nullptr }, mUsrKey{ nullptr }, gMode{} {}

Nyan::Input::Input(const int gMode)
	: gMode{ gMode }, isTerminated{ false }, tKeyListen{ nullptr }, tClkListen{ nullptr } {
	usrKeyStat = new KeySet;
	sysNumStat = new __int16;

	cvUsrKey = new conVar * [gMode];
	for (int i = 0; i < gMode; ++i) {
		cvUsrKey[i] = new conVar;
	}
	cvCmdKey = new conVar;
	cvSysKey = new conVar * [gMode];
	for (int i = 0; i < gMode; ++i) {
		cvSysKey[i] = new conVar;
	}
	cvClock = new conVar;

	mUsrKey = new mutex;
	mSysKey = new mutex;
}

void Nyan::Input::inputFrame(const int period) {
	tKeyListen = new thread{ &Nyan::Input::listenKeyStat, this, NYANIO_PAST };
	tClkListen = new thread{ &Nyan::Input::listenClock, this, period };

	tKeyListen->join();
	tClkListen->join();
}

void Nyan::Input::listenKeyStat(const int option) {
	do {
		chrono::system_clock::time_point start = chrono::system_clock::now();
		mUsrKey->lock();
		usrKeyStat->numKey = 0;
		usrKeyStat->cmdKey = 0;
		mUsrKey->unlock();

		for (int i = 0; i < gMode; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & option) {
				mUsrKey->lock();
				usrKeyStat->numKey |= 0b1 << i;
				mUsrKey->unlock();
				cvUsrKey[i]->notify_one();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & option) {
			mUsrKey->lock();
			usrKeyStat->cmdKey = VK_ESCAPE;
			mUsrKey->unlock();
			cvCmdKey->notify_all();
		}
		this_thread::sleep_until(start + chrono::milliseconds(1));
	} while (true); //TODO: 종료조건 정할것
}

void Nyan::Input::listenClock(const int period) { //TDOO: 클록을 모든 개체가 공유하도록
	do {
		chrono::system_clock::time_point start = chrono::system_clock::now();
		do { // wait for period(ms)
			this_thread::sleep_for(chrono::milliseconds(1));
		} while (chrono::system_clock::now() - start < chrono::milliseconds(period));

		cvClock->notify_all();
	} while (!isTerminated);
}

Nyan::KeySet* Nyan::Input::getKeyStat() {
	return usrKeyStat;
}


Nyan::IOHandler::IOHandler()
	: Input(), tKeyDraw{ nullptr } {}

Nyan::IOHandler::IOHandler(const int gMode)
	: Input(gMode), tKeyDraw{ nullptr } {

	hideCursor(); // Optional
}

void Nyan::IOHandler::outputFrame() {
	tKeyDraw = new thread * [gMode];
	for (int i = 0; i < gMode; ++i) {
		tKeyDraw[i] = new thread{ &Nyan::IOHandler::drawKey, this, i };
	}

	for (int i = 0; i < gMode; ++i) {
		tKeyDraw[i]->join();
	}
}

void Nyan::IOHandler::drawKey(const int numKey) {
	int posX{ numKey % 3 + 1 }, posY{ 3 - numKey / 3 };
	chrono::system_clock::time_point start;

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsrKey);	// Sync to user key
		cvUsrKey[numKey]->wait(mUsrNumKey, [&] { return (usrKeyStat->numKey & (0b1 << numKey)) == (0b1 << numKey); });
		// push to output buffer
		mUsrNumKey.unlock();
	}
}

void Nyan::hideCursor() {
	CONSOLE_CURSOR_INFO cursor{};
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void Nyan::moveCursor(const int x, const int y) {
	COORD cursor;
	cursor.X = x;
	cursor.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}