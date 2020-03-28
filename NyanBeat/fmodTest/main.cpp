#include <bitset>
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

#define NUM_THREAD 16

HANDLE hMutex;

int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	Sound sound{ 3, fileList };
	sound.PlaySoundNo(0);

	HANDLE hThread[NUM_THREAD];
	unsigned threadID{1};

	KeySet currKey{};
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, listenKeyPress, (void*)&currKey, 0, &threadID);

	hMutex = CreateMutex(NULL, FALSE, NULL);
	do {
		WaitForSingleObject(hMutex, INFINITE); // critial section control for keyPress 
		if (currKey.numKey != 0) {
			std::cout << "Current KeySet(num): " << std::bitset<9>(currKey.numKey) << std::endl;
		}
		if (currKey.cmdKey != 0) {
			std::bitset<9> temp(currKey.cmdKey);
			std::cout << "Current KeySet(cmd): " << std::bitset<9>(currKey.cmdKey) << std::endl;
		}
		ReleaseMutex(hMutex); // end
	} while (true);


	WaitForMultipleObjects(NUM_THREAD, hThread, TRUE, INFINITE);

	return 0;
}