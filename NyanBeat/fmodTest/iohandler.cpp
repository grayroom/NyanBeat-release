#include "pch.h"

#include "iohandler.h"
#include "common.h"


// ------------------------------------------------------------------------------------------------

Nyan::KeyHandler::KeyHandler() {
	usrKey->numKey = 0;
	usrKey->cmdKey = 0;
	sysKey->numKey = 0;
	sysKey->cmdKey = 0;
}

Nyan::KeySet*& Nyan::KeyHandler::getUsrKey() {
	return usrKey;
}

void Nyan::KeyHandler::setUsrKey(KeySet*& usrKey) {
	this->usrKey = usrKey;
}

Nyan::KeySet*& Nyan::KeyHandler::getSysKey() {
	return sysKey;
}

void Nyan::KeyHandler::setSysKey(KeySet*& sysKey) {
	this->sysKey = sysKey;
}

// ------------------------------------------------------------------------------------------------

Nyan::Input::Input()
	: KeyHandler(), gMode{ 0 }, isTerminated{ false } {}

Nyan::Input::Input(const int gMode)
	: KeyHandler(), gMode { gMode }, isTerminated{ false } {}

void Nyan::Input::listenUsrKey(const int opt, mutex*& mUsrKey, conVar**& cvNumKey, conVar*& cvCmdKey) {
	chrono::system_clock::time_point start{};

	do {
		start = chrono::system_clock::now();
		mUsrKey->lock();
		usrKey->numKey = 0;
		usrKey->cmdKey = 0;
		mUsrKey->unlock();

		for (int i = 0; i < gMode; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & opt) {
				mUsrKey->lock();
				usrKey->numKey |= 0b1 << i;
				mUsrKey->unlock();
				cvNumKey[i]->notify_one();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & opt) {
			mUsrKey->lock();
			usrKey->cmdKey = VK_ESCAPE;
			mUsrKey->unlock();
			cvCmdKey->notify_all();
		}
		this_thread::sleep_until(start + chrono::milliseconds(1));
	} while (!isTerminated);
}

void Nyan::Input::listenSysKey(fs::path noteDir, mutex*& mSysKey, conVar**& cvNumKey) {
	fstream noteStream{ noteDir, ios::binary | ios::in };
	chrono::system_clock::time_point start{};

	// period, gMode데이터를 읽어옴
	int periodRef, gModeRef;
	noteStream >> periodRef >> gModeRef;

	if (gMode != gModeRef) {
		// 예외처리
	}

	// period를 넘겨줘야 할까?

	do { // 값을 일정시간 먼저 읽어와야함!!
		start = chrono::system_clock::now();

		mSysKey->lock();
		noteStream >> sysKey->numKey;
		mSysKey->unlock();

		for (int i = 0; i < gMode; ++i) {
			if ((sysKey->numKey & (0b1 << i)) == (0b1 << i)) {
				cvNumKey[i]->notify_one();
			}
		}
		this_thread::sleep_until(start + chrono::milliseconds(periodRef));
	} while (!isTerminated);
}

// ------------------------------------------------------------------------------------------------

Nyan::IOHandler::IOHandler()
	: KeyHandler(), gMode{ 0 }, isTerminated{ false } {}

Nyan::IOHandler::IOHandler(const int gMode)
	: KeyHandler(), gMode{ gMode }, isTerminated{ false } {
	hideCursor(); // optional
}

//TODO: sysKey와 usrKey를 동시에 출력할 방법을 생각해보자
void Nyan::IOHandler::drawKey(const int numKey, mutex*& mUsrKey, conVar**& cvNumKey, conVar*& cvCmdKey) {
	int posX{ numKey % 3 + 1 }, posY{ 3 - numKey / 3 };

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsrKey); // Sync to user key
		cvNumKey[numKey]->wait(mUsrNumKey, [&] { return (usrKey->numKey & (0b1 << numKey)) == (0b1 << numKey); });
		
		//TODO: push to output buffer
		
		mUsrNumKey.unlock();
	}
}

// ------------------------------------------------------------------------------------------------



// ------------------------------------------------------------------------------------------------

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