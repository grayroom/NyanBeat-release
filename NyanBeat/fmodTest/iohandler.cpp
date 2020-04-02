#include "pch.h"

#include "iohandler.h"
#include "common.h"


// ------------------------------------------------------------------------------------------------

Nyan::KeyHandler::KeyHandler()
	: numKey{ 0 }, mUsr{ new mutex }, mSys{ new mutex }, cvUsrNum{ new conVar }, cvSysNum{ new conVar }, cvUsrCmd{ new conVar } {
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

	cvUsrNum = cvs[0];
	cvSysNum = cvs[1];
	cvUsrCmd = cvs[2];
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
	: KeyHandler(), clkPeriod{ 0 }, isTerminated{ false } {}

Nyan::Input::Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey)
	: KeyHandler(usrKey, sysKey, numKey), clkPeriod{}, isTerminated{ false } {}

Nyan::Input::Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), clkPeriod{}, isTerminated{ false } {}

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
				cvUsrNum->notify_all();
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
				cvSysNum->notify_all();
			}
		}
		this_thread::sleep_until(start + chrono::milliseconds(clock));
	} while (!isTerminated);
}

// ------------------------------------------------------------------------------------------------

Nyan::IOHandler::IOHandler()
	: KeyHandler(), clkPeriod{ 0 }, isTerminated{ false } {}

Nyan::IOHandler::IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey)
	: KeyHandler(usrKey, sysKey, numKey), clkPeriod{}, isTerminated{ false } {
	hideCursor(); // optional
}

Nyan::IOHandler::IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), clkPeriod{}, isTerminated{ false } {
	hideCursor();
}

//TODO: sysKey와 usrKey를 동시에 출력할 방법을 생각해보자
void Nyan::IOHandler::drawKey(const int keyNum, __int8*& keyBuf, mutex*& mKeyBuf) {
	int keyPhase{ 9 };

	while (!isTerminated) {
		{
			unique_lock<mutex> mUsrNumKey(*mUsr); // Sync to user key
			cvUsrNum->wait(mUsrNumKey, [&] { return (usrKey->numKey & (0b1 << keyNum)) == (0b1 << keyNum); });
		}

		for (int i = 0; i < MAX_KEY_PHASE; ++i) {
			mKeyBuf->lock();
			keyBuf[keyNum] = MAX_KEY_PHASE - i;
			mKeyBuf->unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------------

Nyan::Output::Output()
	: keyBuf{ new __int8* }, numKey{}, clkPeriod{}, mKeyBuf{ new mutex }, isTerminated{ false } {}

Nyan::Output::Output(const int numKey)
	: keyBuf{ new __int8* [numKey] }, numKey{ numKey }, clkPeriod{}, mKeyBuf{ new mutex }, isTerminated{ false } {}

void Nyan::Output::drawConsoleNote() {
	chrono::system_clock::time_point start{ chrono::system_clock::now() };
	
	do {
		this_thread::sleep_until(start + chrono::milliseconds(clkPeriod)); // clkPeriod를 int형에서 chrono::miliseconds형으로 바꿀까
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