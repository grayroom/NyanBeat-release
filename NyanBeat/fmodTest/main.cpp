#include "pch.h"
#include "common.h"
#include "resource.h"
#include "iohandler.h"
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

#define NUM_THREAD 16

extern HANDLE hMutex_keyStat;

int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	Sound sound{ 3, fileList };
	hMutex_keyStat = CreateMutex(NULL, FALSE, NULL);

	HANDLE hThread[NUM_THREAD];
	unsigned threadID{1};

	NyanIO::Input hInput{};
	int option{ NYANIO_KEEP };
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, NyanIO::Input::listenKeyStat, (void*)&option, 0, &threadID);

	sound.PlaySoundNo(0);

	do {
		WaitForSingleObject(hMutex_keyStat, INFINITE); // critial section control for keyPress 
		if (hInput.getKeyStat().numKey != 0) {
			std::cout << "Current KeySet(num): " << std::bitset<9>(hInput.getKeyStat().numKey) << std::endl;
		}
		if (hInput.getKeyStat().cmdKey != 0) {
			std::cout << "Current KeySet(cmd): " << std::bitset<9>(hInput.getKeyStat().cmdKey) << std::endl;
		}
		ReleaseMutex(hMutex_keyStat); // end
	} while (true);


	WaitForMultipleObjects(NUM_THREAD, hThread, TRUE, INFINITE);

	return 0;
}