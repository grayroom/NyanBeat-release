#include "pch.h"

#include "iohandler.h"
#include "common.h"


// ------------------------------------------------------------------------------------------------

Nyan::KeyHandler::KeyHandler()
	: numKey{ 0 }, mUsr{ new mutex }, mSys{ new mutex }, cvUsrNum{ new conVar }, cvSysNum{ new conVar }, cvUsrCmd{ new conVar } {
	usrKey = new KeySet;
	sysKey = new KeySet;
}

Nyan::KeyHandler::KeyHandler(const int numKey)
	: KeyHandler() {
	this->numKey = numKey;
}

Nyan::KeyHandler::KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs)
	: usrKey{ usrKey }, sysKey{ sysKey }, numKey{ numKey } {
	mUsr = ms[0];
	mSys = ms[1];

	cvUsrNum = cvs[0];
	cvSysNum = cvs[1];
	cvUsrCmd = cvs[2];
}

Nyan::KeySet* Nyan::KeyHandler::getUsrKey() {
	return usrKey;
}

void Nyan::KeyHandler::setUsrKey(KeySet*& usrKey) {
	this->usrKey = usrKey;
}

Nyan::KeySet* Nyan::KeyHandler::getSysKey() {
	return sysKey;
}

void Nyan::KeyHandler::setSysKey(KeySet*& sysKey) {
	this->sysKey = sysKey;
}

mutex** Nyan::KeyHandler::getMutexList() {
	mutex** mList{ new mutex * [2] };
	mList[0] = mUsr;
	mList[1] = mSys;

	return mList;
}

void Nyan::KeyHandler::setMutexList(mutex**& mList) {
	mUsr = mList[0];
	mSys = mList[1];
}

conVar** Nyan::KeyHandler::getConvarList() {
	conVar** cvList{ new conVar * [3] };
	cvList[0] = cvUsrNum;
	cvList[1] = cvSysNum;
	cvList[2] = cvUsrCmd;

	return cvList;
}

void Nyan::KeyHandler::setConvarList(conVar**& cvList) {
	cvUsrNum = cvList[0];
	cvSysNum = cvList[1];
	cvUsrCmd = cvList[2];
}

// ------------------------------------------------------------------------------------------------

Nyan::Input::Input()
	: KeyHandler(), clkPeriod{ 200 }, isTerminated{} {}

Nyan::Input::Input(const int numKey)
	: KeyHandler(numKey), clkPeriod{ 200 }, isTerminated{} {}

Nyan::Input::Input(KeySet* usrKey, KeySet* sysKey, const int numKey, mutex** ms, conVar** cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), clkPeriod{ 200 }, isTerminated{} {}

void Nyan::Input::listenUsrKey(const int opt) {
	chrono::system_clock::time_point start{};

	do {
		start = chrono::system_clock::now();
		mUsr->lock();
		usrKey->num = 0;
		usrKey->cmd = 0;
		mUsr->unlock();

		for (int i = 0; i < numKey; ++i) {
			if (GetAsyncKeyState(VK_NUMPAD1 + i) & opt) {
				mUsr->lock();
				usrKey->num |= (0b1 << i);
				mUsr->unlock();
				cvUsrNum->notify_all();
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE) & opt) {
			mUsr->lock();
			usrKey->cmd = VK_ESCAPE;
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
	int gModeRef;
	noteStream >> clkPeriod >> gModeRef;

	if (numKey != gModeRef) {
		// 예외처리
	}

	// Trigger를 설계할 것

	do { // 값을 일정시간 먼저 읽어와야함!!
		start = chrono::system_clock::now();

		mSys->lock();
		noteStream >> sysKey->num;
		mSys->unlock();

		for (int i = 0; i < numKey; ++i) {
			if ((sysKey->num & (0b1 << i)) == (0b1 << i)) {
				cvSysNum->notify_all();
			}
		}
		this_thread::sleep_until(start + chrono::milliseconds(clkPeriod));
	} while (!isTerminated);
}

// ------------------------------------------------------------------------------------------------

Nyan::Output::Output()
	: keyBuf{ new __int8{} }, numKey{}, clkPeriod{ 200 }, mKeyBuf{ new mutex }, isTerminated{} {}

Nyan::Output::Output(const int numKey)
	: keyBuf{ new __int8[numKey] {} }, numKey{ numKey }, clkPeriod{ 200 }, mKeyBuf{ new mutex }, isTerminated{} {}

__int8* Nyan::Output::getKeyBuf() {
	return keyBuf;
}

void Nyan::Output::setKeyBuf(__int8* keyBuf) {
	this->keyBuf = keyBuf;
}

mutex* Nyan::Output::getMKeyBuf() {
	return mKeyBuf;
}

void Nyan::Output::setMKeyBuf(mutex* mKeyBuf) {
	this->mKeyBuf = mKeyBuf;
}

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
	int posX{ keyNum % 3 + 1 + 3 }, posY{ 3 - keyNum / 3 + 3 };

	moveCursor(posX, posY);
	cout << (char)(keyBuf[keyNum] + 48);
}

// ------------------------------------------------------------------------------------------------

Nyan::IOHandler::IOHandler()
	: KeyHandler(), clkPeriod{ 200 }, isTerminated{} {}

Nyan::IOHandler::IOHandler(const int numKey)
	: KeyHandler(numKey), clkPeriod{ 200 }, isTerminated{} {}

Nyan::IOHandler::IOHandler(KeySet* usrKey, KeySet* sysKey, const int numKey, mutex** ms, conVar** cvs)
	: KeyHandler(usrKey, sysKey, numKey, ms, cvs), clkPeriod{ 200 }, isTerminated{} {
	hideCursor();
}

//TODO: sysKey와 usrKey를 동시에 출력할 방법을 생각해보자
void Nyan::IOHandler::drawKey(const int keyNum, __int8* keyBuf, mutex* mKeyBuf) {
	chrono::system_clock::time_point start{ chrono::system_clock::now() };

	while (!isTerminated) {
		{
			unique_lock<mutex> mUsrNumKey(*mUsr); // Sync to user key
			cvUsrNum->wait(mUsrNumKey, [&] {
				return (usrKey->num & (0b1 << keyNum)) == (0b1 << keyNum); 
			});
			usrKey->cmd &= ~(0b1 << keyNum);
		}
		for (int i = 0; i < MAX_KEY_PHASE; ++i) {
			if ((usrKey->num & (0b1 << keyNum)) == (0b1 << keyNum)) {
				i = 0;
			}
			mKeyBuf->lock();
			keyBuf[keyNum] = MAX_KEY_PHASE - i - 1;
			mKeyBuf->unlock();

			this_thread::sleep_until(start = start + chrono::milliseconds(clkPeriod));
		}
	}
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