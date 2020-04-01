#pragma once

#include "resource.h"

#define NYANIO_NEVER	0x0000
#define NYANIO_NOW		0x0001
#define NYANIO_PAST		0x8000
#define NYANIO_KEEP		0x8001

#define GMODE_9KEY		9
#define GMDOE_6KEY		6
#define GMODE_3KEY		3

using namespace std;

typedef condition_variable conVar;

namespace Nyan {
	struct KeySet {
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

	class KeyHandler {
	protected:
		KeySet* usrKey;
		KeySet* sysKey;

	public:
		KeyHandler();
		KeyHandler(KeySet*& usrKey, KeySet*& sysKey);

		KeySet*&	getUsrKey();
		void		setUsrKey(KeySet*& keySet);
		KeySet*&	getSysKey();
		void		setSysKey(KeySet*& keySet);
	};

	class Input : public KeyHandler {
	private:

		int		gMode;

		bool	isTerminated;

	public:
		Input();
		Input(const int gMode);
		Input(const int gMode, KeySet*& usrKey, KeySet*& sysKey);

		// virtual함수를 선언해야하나

		void	listenUsrKey(const int opt, mutex*& mUsrKey, conVar**& cvNumKey, conVar*& cvCmdKey);
		void	listenSysKey(fs::path noteDir, mutex*& mSysKey, conVar**& cvNumKey);
	};

	class Output {
	private:
		__int8** keyStat{};

	public:
		Output();
	};

	class IOHandler : public KeyHandler {
	private:
		int			gMode;

		bool		isTerminated;

	public:
		IOHandler();
		IOHandler(const int gMode);
		IOHandler(const int gMode, KeySet*& usrKey, KeySet*& sysKey);

		// virtual함수를 선언해야하나

		void drawKey(const int numKey, mutex*& mUsrKey, conVar**& cvNumKey, conVar*& cvCmdKey);
	};

	void hideCursor();
	void moveCursor(const int x, const int y);
}
