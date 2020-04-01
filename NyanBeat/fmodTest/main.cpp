#include "pch.h"
#include "common.h"
#include "resource.h"
#include "iohandler.h"
#include "sound.h"

#define TRACK 3
#define GMODE 9



int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	
	Sound sound{ TRACK, fileList };
	sound.PlaySoundNo(0);

	Nyan::Input* pInput{ new Nyan::Input{GMODE} };
	Nyan::IOHandler* pIOHandler{ new Nyan::IOHandler{GMODE, pInput->getUsrKey(), pInput->getSysKey()} };

	mutex* mUsrKey{ new mutex },
		* mSysKey{ new mutex };

	conVar** cvUsrNumKey{},
		** cvSysNumKey{},
		* cvUsrCmdKey{};

	thread** hListenKey{ new thread * [2] };
	hListenKey[0] = new thread{ &Nyan::Input::listenUsrKey, pInput , NYANIO_NOW, mUsrKey, cvUsrNumKey, cvUsrCmdKey};
	//hListenKey[1] = new thread{ &Nyan::Input::listenSysKey, pIOHandler, /*path argument*/, mSysKey, cvSysNumKey};

	for (thread* xThread : hThreads) {
		xThread->join();
	}

	/*do {
		if (pInput->getKeyStat()->numKey != 0) {
			std::cout << "Current KeySet(num): " << std::bitset<9>(pInput->getKeyStat()->numKey) << std::endl;
		}
		if (pInput->getKeyStat()->cmdKey != 0) {
			std::cout << "Current KeySet(cmd): " << std::bitset<9>(pInput->getKeyStat()->cmdKey) << std::endl;
		}
	} while (true);*/

	return 0;
}