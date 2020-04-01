#include "pch.h"

#include "iohandler.h"
#include "common.h"


Nyan::Input::Input()
	: gMode{ 0 }, isTerminated{ false } {
	usrKey = 0;
	sysKey = 0;
}

Nyan::Input::Input(const int gMode)
	: gMode{ gMode }, isTerminated{ false } {
	usrKey = 0;
	sysKey = 0;
}

Nyan::KeySet*& Nyan::Input::getUsrKey() {
	return usrKey;
}

void Nyan::Input::setUsrKey(KeySet*& usrKey) {
	this->usrKey = usrKey;
}

Nyan::KeySet*& Nyan::Input::getSysKey() {
	return sysKey;
}

void Nyan::Input::setSysKey(KeySet*& sysKey) {
	this->sysKey = sysKey;
}

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

void Nyan::Input::listenSysKey(fs::path noteDir, mutex*& mSysKey, conVar*& cvNumKey) {
	fstream noteStream{ noteDir, ios::binary | ios::in };
	chrono::system_clock::time_point start{};

	// period, gMode데이터를 읽어옴
	int periodRef, gModeRef;
	noteStream >> periodRef >> gModeRef;
	do {
		start = chrono::system_clock::now();

		mSysKey->lock();
		noteStream >> sysKey->numKey;
		mSysKey->unlock();

		this_thread::sleep_until(start + chrono::milliseconds(periodRef));
	} while (!isTerminated);
}

// --------------------------------------------------------------------------------------------

Nyan::IOHandler::IOHandler()
	: gMode{ 0 }, isTerminated{ false } {
	usrKey = 0;
	sysKey = 0;
}

Nyan::IOHandler::IOHandler(const int gMode)
	: gMode{ gMode }, isTerminated{ false } {
	usrKey = 0;
	sysKey = 0;

	hideCursor(); // Optional
}

void Nyan::IOHandler::drawKey(const int numKey, mutex*& mUsrKey, conVar**& cvNumKey, conVar*& cvCmdKey) {
	int posX{ numKey % 3 + 1 }, posY{ 3 - numKey / 3 };

	while (!isTerminated) {
		unique_lock<mutex> mUsrNumKey(*mUsrKey); // Sync to user key
		cvNumKey[numKey]->wait(mUsrNumKey, [&] { return (usrKey->numKey & (0b1 << numKey)) == (0b1 << numKey); });
		//TODO: push to output buffer
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