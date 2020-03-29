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
		Keyset*	keyStat;
		int		gMode;

		std::mutex* mUsrKey;

	public:
		Input();
		Input(NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode);

		void listenKeyStat(const int option);

		Keyset getKeyStat();
	};

	class Output {
	private:
		Keyset*		keyStat;
		__int16*	sysNumKey;

		int			gMode;
		int			tick;

		bool		isTerminated;

		std::thread**	hKeyThread{};
		std::thread*	hUsrThread{};
		std::thread*	hSysThread{};

		
		std::condition_variable*	cvUsrKey;
		std::condition_variable		cvCmdKey;
		std::condition_variable*	cvSysKey;
		std::condition_variable		cvClock;

		std::mutex* mUsrKey;
		std::mutex	mGlobTick;

	public:
		Output();
		Output(NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode);

		void outputFrame();

		void listenSysKeyStat();
		void listenUsrKeyStat();
		void listenClock(const int period);
		void drawKey(const int numKey);
	};

	void initNyanIO(Input* pInput, Output* pOutput, NyanIO::Keyset* keyStat, std::mutex* pMutex, const int gMode);

	void hideCursor();
	void moveCursor(const int x, const int y);
}
