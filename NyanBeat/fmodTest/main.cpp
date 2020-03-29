#include "pch.h"
#include "common.h"
#include "resource.h"
#include "iohandler.h"
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

#define NUM_THREAD 16

extern std::mutex mUsrKey;

int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	Sound sound{ 3, fileList };

	std::vector<std::thread> hThread{};

	NyanIO::Input hInput; // static member of NyanIO::Input class
	int option{ NYANIO_KEEP };
	hThread.emplace_back(&NyanIO::Input::listenKeyStat, &hInput, option);
	sound.PlaySoundNo(0);

	do {
		mUsrKey.lock();
		if (hInput.getKeyStat().numKey != 0) {
			std::cout << "Current KeySet(num): " << std::bitset<9>(hInput.getKeyStat().numKey) << std::endl;
		}
		if (hInput.getKeyStat().cmdKey != 0) {
			std::cout << "Current KeySet(cmd): " << std::bitset<9>(hInput.getKeyStat().cmdKey) << std::endl;
		}
		mUsrKey.unlock();
	} while (true);

	return 0;
}