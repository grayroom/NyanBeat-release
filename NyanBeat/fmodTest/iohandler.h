#pragma once

#include "resource.h"


#define NYANIO_NEVER	0x0000
#define NYANIO_NOW		0x0001
#define NYANIO_PAST		0x8000
#define NYANIO_KEEP		0x8001

namespace NyanIO {
	struct Keyset {
		/*
			[--------EMPTY 22 FIELD--------][987654321]
			Only lower 9 bit has valid key value. When number key pressed, corresponding bit set high(1). In this way, you can
			get 9 key status information at the same time(9-key rollover)
		*/
		__int16 numKey = 0;
		/*
			Get single command key
		*/
		__int8 cmdKey = 0;
	};

	class Input {
	private:
		static Keyset keyStat;

	public:
		static unsigned WINAPI listenKeyStat(void* arg);

		static Keyset getKeyStat();
	};

	class Output {
	private:
		__int16*	usrKey;
		__int8*		cmdKey;
		__int16*	sysKey;

		int keyPos[9][2];

		HANDLE hKeyThread[9];
		unsigned keyTid[9];
		

	public:
		Output();
		Output(NyanIO::Keyset& keyStat);
		void drawSysKey();
		void getUsrKey(NyanIO::Keyset& keyStat);
		static unsigned WINAPI DrawAnimatedKey(void* arg);
		static unsigned WINAPI listenUsrKey(void* arg);
	};

	void hideCursor();
	void moveCursor(const int x, const int y);
}
