#include "pch.h"

#include "iohandler.h"
#include "common.h"


// ------------------------------------------------------------------------------------------------

Nyan::KeyHandler::KeyHandler()
	: clock{ 0 }, mUsr{ new mutex }, mSys{ new mutex }, cvUsrNum{ new conVar* }, cvSysNum{ new conVar* }, cvUsrCmd{ new conVar }, cvClock{ new conVar } {
	usrKey->numKey = 0;
	usrKey->cmdKey = 0;
	sysKey->numKey = 0;
	sysKey->cmdKey = 0;
}

Nyan::KeyHandler::KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey) 
	: KeyHandler() {
	this->usrKey = usrKey;
	this->usrKey = sysKey;
	this->numKey = numKey;
}

Nyan::KeyHandler::KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: KeyHandler(usrKey, sysKey, numKey) {
	mUsr = ms[0];
	mSys = ms[1];

	int i{ 0 };
	cvUsrNum = new conVar * [numKey];
	for (int j = 0; j < numKey; ++i, ++j) {
		cvUsrNum[j] = cvs[i];
	}
	cvSysNum = new conVar * [numKey];
	for (int j = 0; j < numKey; ++i, ++j) {
		cvSysNum[j] = cvs[i];
	}
	cvUsrCmd = cvs[i++];
	cvClock = cvs[i];
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
	: KeyHandler(), numKey{ 0 }, isTerminated{ false } {}

Nyan::Input::Input(const int gMode)
	: KeyHandler(), numKey { numKey }, isTerminated{ false } {}

Nyan::Input::Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey)
	: KeyHandler(usrKey, sysKey, numKey), isTerminated{ false } {}

Nyan::Input::Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), isTerminated{ false } {}

void Nyan::Input::listenUsrKey(const int opt) {
	chrono::system_clock::time_point start{};

	do {
		start = chrono::system_clock::now();
		mUsr->lock();
		usrKey->numKey = 0;
		usrKey->cmdKey = 0;
		mUsr->unlock();

		for (int i = 0; i < numKey; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & opt) {
				mUsr->lock();
				usrKey->numKey |= 0b1 << i;
				mUsr->unlock();
				cvUsrNum[i]->notify_one();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & opt) {
			mUsr->lock();
			usrKey->cmdKey = VK_ESCAPE;
			mUsr->unlock();
			cvUsrCmd->notify_all();
		}
		this_thread::sleep_until(start + chrono::milliseconds(1));
	} while (!isTerminated);
}

void Nyan::Input::listenSysKey(fs::path noteDir) {
	fstream noteStream{ noteDir, ios::binary | ios::in };
	chrono::system_clock::time_point start{};

	// period, gMode데이터를 읽어옴
	int clock, gModeRef;
	noteStream >> clock >> gModeRef;

	if (numKey != gModeRef) {
		// 예외처리
	}

	do { // 값을 일정시간 먼저 읽어와야함!!
		start = chrono::system_clock::now();

		mSys->lock();
		noteStream >> sysKey->numKey;
		mSys->unlock();

		for (int i = 0; i < numKey; ++i) {
			if ((sysKey->numKey & (0b1 << i)) == (0b1 << i)) {
				cvSysNum[i]->notify_one();
			}
		}
		this_thread::sleep_until(start + chrono::milliseconds(clock));
	} while (!isTerminated);
}

void Nyan::Input::listenClock() {
	chrono::system_clock::time_point start{ chrono::system_clock::now() };

	do {
		this_thread::sleep_until(start = start + chrono::milliseconds(clock));

		cvClock->notify_all();
	} while (!isTerminated);
}

// ------------------------------------------------------------------------------------------------

Nyan::IOHandler::IOHandler()
	: KeyHandler(), numKey{ 0 }, isTerminated{ false } {}

Nyan::IOHandler::IOHandler(const int gMode)
	: KeyHandler(), numKey{ gMode }, isTerminated{ false } {
	hideCursor(); // optional
}

Nyan::IOHandler::IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey)
	: KeyHandler(usrKey, sysKey, numKey), isTerminated{ false } {
	hideCursor(); // optional
}

Nyan::IOHandler::IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), isTerminated{ false } {
	hideCursor();
}

//TODO: sysKey와 usrKey를 동시에 출력할 방법을 생각해보자
void Nyan::IOHandler::drawKey(const int keyNum, __int8*& keyBuf, mutex*& mKeyBuf) {
	int keyPhase{ 9 };

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsr); // Sync to user key
		cvUsrNum[keyNum]->wait(mUsrNumKey);

		for (int i = 0; i < keyNum; ++i) {
			unique_lock<mutex> clockSync{ *mKeyBuf };
			cvClock->wait(clockSync);
			keyBuf[keyNum] = keyPhase--;
			clockSync.unlock();
		}

		mUsrNumKey.unlock();
	}
}

// ------------------------------------------------------------------------------------------------

Nyan::Output::Output()
	: keyBuf{ new __int8* }, numKey{}, cvClock{ new conVar }, isTerminated{ false } {}

Nyan::Output::Output(const int numKey, conVar*& cvClock)
	: keyBuf{ new __int8* [numKey] }, numKey{ numKey }, cvClock{ cvClock }, isTerminated{ false } {}

void Nyan::Output::drawConsoleNote() {
	do {
		unique_lock<mutex> clockSync{ *mKeyBuf };
		cvClock->wait(clockSync);
		clockSync.unlock();

		for (int i = 0; i < numKey; ++i) {
			drawNote(i);
		}
	} while (!isTerminated);
}

void Nyan::Output::drawNote(const int keyNum) {
	int posX{ keyNum % 3 + 1 }, posY{ 3 - keyNum / 3 };

	moveCursor(posX, posY);
	cout << *keyBuf[keyNum];
}

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