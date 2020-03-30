#pragma once

#include "resource.h"

#include <thread>

#define NYANIO_NEVER	0x0000
#define NYANIO_NOW		0x0001
#define NYANIO_PAST		0x8000
#define NYANIO_KEEP		0x8001

#define GMODE_9KEY		9
#define GMDOE_6KEY		6
#define GMODE_3KEY		3

using namespace std;

typedef condition_variable conVar;

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
		Keyset*	keyUsrStat;
		int		gMode;

		bool		isTerminated;

		thread*	tKeyListen{};
		thread*	tClock{};
		
		conVar**	cvUsrKey;
		conVar*		cvCmdKey;
		conVar**	cvSysKey;
		conVar*		cvClock;

		mutex*	mUsrKey;
		mutex*	mSysKey;

	public:
		Input();
		Input(NyanIO::Keyset*& keyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode);

		void inputFrame(const int period);
		void listenKeyStat(const int option);
		void listenClock(const int clock);

		Keyset* getKeyStat();
	};

	class Output {
	private:
		Keyset*		keyUsrStat;
		__int16*	sysNumKey;

		int			gMode;
		int			tick;

		bool		isTerminated;

		// thread handles
		vector<thread*>	hThreads{};
		thread**	tKeyDraw{};
		
		// condition variables
		conVar**	cvUsrKey;
		conVar*		cvCmdKey;
		conVar**	cvSysKey;
		conVar*		cvClock;

		// mutexes
		vector<mutex*> hMutex{};
		mutex*	mUsrKey;
		mutex*	mSysKey;
		mutex*	mKeyDraw;

	public:
		Output();
		Output(NyanIO::Keyset*& keyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode);

		void outputFrame();

		void drawKey(const int numKey);
	};

	void initNyanIO(Input*& pInput, Output*& pOutput, NyanIO::Keyset*& keyStat, vector<mutex*>& hMutex, vector<conVar*>& cvs, const int gMode);

	void hideCursor();
	void moveCursor(const int x, const int y);
}
