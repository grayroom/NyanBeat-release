#include "pch.h"
#include "common.h"
#include "resource.h"
#include "iohandler.h"
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

#define NUM_THREAD 16

int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	
	Sound sound{ 3, fileList };
	sound.PlaySoundNo(0);

	NyanIO::Input* pInput{};
	NyanIO::Output* pOutput{};
	NyanIO::Keyset* pKeySet{};
	vector<mutex*> hMutex{};
	vector<conVar*> cvs{};

	NyanIO::initNyanIO(pInput, pOutput, pKeySet, hMutex, cvs, GMODE_9KEY);

	vector<thread*> hThreads{};
	hThreads.push_back(new thread{ &NyanIO::Input::inputFrame, &*pInput, 200 });
	hThreads.push_back(new thread{ &NyanIO::Output::outputFrame, &*pOutput });

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