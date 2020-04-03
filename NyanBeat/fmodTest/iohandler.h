#pragma once

#include "resource.h"

#define NYANIO_NEVER	0x0000
#define NYANIO_NOW		0x0001
#define NYANIO_PAST		0x8000
#define NYANIO_KEEP		0x8001

#define GMODE_9KEY		9
#define GMDOE_6KEY		6
#define GMODE_3KEY		3

#define MAX_KEY_PHASE	9

using namespace std;

typedef condition_variable conVar;

namespace Nyan {
	struct KeySet {
		/*
			[--------EMPTY 22 FIELD--------][987654321]
			Only lower 9 bit has valid key value. When number key pressed, corresponding bit set high(1).
			In this way, we can get 9 key status information at the same time(9-key rollover)
		*/
		__int16 num = 0;
		/*
			Get single command key
		*/
		__int8 cmd = 0;
	};

	class KeyHandler {
	protected:
		KeySet* usrKey;
		KeySet* sysKey;

		int numKey;
		//TODO: mutex & condition_variable을 여기에 저장해둘지 생각해볼것
		mutex* mUsr;
		mutex* mSys;

		conVar* cvUsrNum;
		conVar* cvSysNum;
		conVar* cvUsrCmd;

	public:
		KeyHandler();
		KeyHandler(const int numKey);
		KeyHandler(KeySet*& usrKey, KeySet*& sysKey, const int numKey, mutex**& ms, conVar**& cvs);

		KeySet*		getUsrKey();
		void		setUsrKey(KeySet*& keySet);
		KeySet*		getSysKey();
		void		setSysKey(KeySet*& keySet);
		mutex**		getMutexList();
		void		setMutexList(mutex**& mList);
		conVar**	getConvarList();
		void		setConvarList(conVar**& cvList);
	};

	class Input : public KeyHandler {
	private:
		int		clkPeriod;

		bool	isTerminated;

	public:
		Input();
		Input(const int numKey);
		Input(KeySet* usrKey, KeySet* sysKey, const int numKey, mutex** ms, conVar** cvs);

		void	listenUsrKey(const int opt);
		void	listenSysKey(fs::path noteDir);
	};

	class Output {
	private:
		__int8* note;

		int		numKey;
		int		clkPeriod;

		mutex* mKeyBuf;

		bool isTerminated;

	public:
		Output();
		Output(const int numKey);

		__int8* getKeyBuf();
		void setKeyBuf(__int8* keyBuf);
		mutex* getMKeyBuf();
		void setMKeyBuf(mutex* mKeyBuf);

		void drawConsoleNote();
		void drawNote(const int keyNum);
	};

	class IOHandler : public KeyHandler {
	private:
		int		clkPeriod;

		bool	isTerminated;

	public:
		IOHandler();
		IOHandler(const int numKey);
		IOHandler(KeySet* usrKey, KeySet* sysKey, const int numKey, mutex** ms, conVar** cvs);

		// virtual함수를 선언해야하나

		void drawKey(const int keyNum, __int8* note, mutex* mNote);
	};

	void hideCursor();
	void moveCursor(const int x, const int y);
}
