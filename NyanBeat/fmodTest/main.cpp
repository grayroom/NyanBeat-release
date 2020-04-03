#include "pch.h"
#include "common.h"
#include "resource.h"
#include "iohandler.h"
#include "sound.h"

#define TRACK 3


int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	
	Sound sound{ TRACK, fileList };
	sound.PlaySoundNo(0);

	// ------------------------------------------------------------------------------------------------

	Nyan::Input* pInput{ new Nyan::Input{GMODE_9KEY} };
	mutex** mList{ pInput->getMutexList() };
	conVar** cvList{ pInput->getConvarList() };

	Nyan::Output* pOutput{ new Nyan::Output{GMODE_9KEY} };

	Nyan::IOHandler* pIOHandler{ new Nyan::IOHandler{pInput->getUsrKey(), pInput->getSysKey(), GMODE_9KEY, mList, cvList} };

	thread** hListenKey{ new thread * [2] };
	hListenKey[0] = new thread{ &Nyan::Input::listenUsrKey, pInput , NYANIO_KEEP};
	//hListenKey[1] = new thread{ &Nyan::Input::listenSysKey, pIOHandler, /*path argument*/};

	thread* hConsole = new thread{ &Nyan::Output::drawConsole, pOutput };
	
	thread** hDrawKey{ new thread * [GMODE_9KEY] };
	for (int i = 0; i < GMODE_9KEY; ++i) {
		hDrawKey[i] = new thread{ &Nyan::IOHandler::drawUsrKey, pIOHandler, i, pOutput->getKeyBuf(), pOutput->getMKeyBuf() };
	}

	for (int i = 0; i < GMODE_9KEY; ++i) {
		hDrawKey[i]->join();
	}
	hConsole->join();
	//hListenKey[1]->join();
	hListenKey[0]->join();

	// ------------------------------------------------------------------------------------------------

	return 0;
}