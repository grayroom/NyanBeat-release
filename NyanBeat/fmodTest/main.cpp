#include <iostream>
#include <Windows.h>
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

int main() {
	vector<fs::path> fileList{ getFiles("../source/sound") };

	Sound sound{ 3, fileList };

	sound.PlaySoundNo(0);

	do {
		if (GetAsyncKeyState('D') >> 1 != 0) {
			sound.ToggleDspEffect(sound.getDspEcho());
		}
	} while (!(GetAsyncKeyState('Q') & 0x8000));

	return 0;
}