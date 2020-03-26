#include <iostream>
#include <Windows.h>
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

int main() {
	vector<fs::path> fileList{ getFiles("../source/sound") };

	Sound sound{ 3, fileList };

	sound.PlaySoundNo(0);

	bool isPlaying{ false };
	do {
		sound.getChannel()->isPlaying(&isPlaying);
		Sleep(5000);
	} while (isPlaying);

	return 0;
}