#include <iostream>
#include "sound.h"

#define TRACK 3
#define TRACK_SIZE 256

int main() {
	vector<fs::path> fileList{ getFiles("../source/sound") };

	for (fs::path entry : fileList) {
		cout << entry << endl;
	}

	Sound soundSys{ 3, fileList };

	return 0;
}