#pragma once

#include "resource.h"

#define NYANIO_NEVER	0x0000
#define NYANIO_NOW		0x0001
#define NYANIO_PAST		0x8000
#define NYANIO_KEEP		0x8001

#define GMODE_9KEY		9
#define GMDOE_6KEY		6
#define GMODE_3KEY		3

#define MAX_KEY_PHASE		9

using namespace std;

typedef condition_variable conVar;

namespace Nyan {
	struct KeySet {
		/*
			[--------EMPTY 22 FIELD--------][987654321]
			Only lower 9 bit has valid key value. When number key pressed, corresponding bit set high(1).
			In this way, we can get 9 key status information at the same time(9-key rollover)
		*/
		__int16 numKey = 0;
		/*
			Get single command key
		*/
		__int8 cmdKey = 0;
	};

	class KeyHandler {
	protected:
		KeySet* usrKey;
		KeySet* sysKey;

		int numKey;
		int clock;
		//TODO: mutex & condition_variable을 여기에 저장해둘지 생각해볼것
		mutex*& mUsr;
		mutex*& mSys;

		conVar**& cvUsrNum;
		conVar**& cvSysNum;
		conVar*& cvUsrCmd;
		conVar*& cvClock;

	public:
		KeyHandler();
		KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey);
		KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs);

		KeySet*&	getUsrKey();
		void		setUsrKey(KeySet*& keySet);
		KeySet*&	getSysKey();
		void		setSysKey(KeySet*& keySet);
	};

	class Input : public KeyHandler {
	private:
		int		clock;
		int		numKey;

		bool	isTerminated;

	public:
		Input();
		Input(const int numKey);
		Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey);
		Input(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs);

		void	listenUsrKey(const int opt);
		void	listenSysKey(fs::path noteDir);
		void	listenClock()
	};

	class Output {
	private:
		__int8** keyBuf;
		int numKey;

		conVar*& cvClock;

		bool isTerminated;

	public:
		Output();
		Output(const int numKey, conVar*& cvClock);

		void printConsole();
	};

	class IOHandler : public KeyHandler {
	private:
		int		clock;
		int		numKey;

		bool	isTerminated;

	public:
		IOHandler();
		IOHandler(const int gMode);
		IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int gMode);
		IOHandler(KeySet*& usrKey, KeySet*& sysKey, const int gMode, mutex**& ms, conVar**& cvs);

		// virtual함수를 선언해야하나

		void drawKey(const int numKey, __int8*& keyBuf, mutex*& mKeyBuf);
	};

	void hideCursor();
	void moveCursor(const int x, const int y);
}
