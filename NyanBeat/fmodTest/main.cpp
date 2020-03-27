#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

int main() {
	std::vector<fs::path> fileList{ getFiles("../source/sound") };
	Sound sound{ 3, fileList };
	sound.PlaySoundNo(0);

	HANDLE hThread;
	unsigned threadID{1};

	char recentKey{};
	hThread = (HANDLE)_beginthreadex(NULL, 0, listenKeyPress, (void*)&recentKey, 0, &threadID);

	short isKey{ false };

	do {
		Sleep(1000);
		std::cout << "waiting..." << std::endl;
	} while (true);

	return 0;
}