#include "sound.h"

std::vector<char *> soundPathList{};

Sound::Sound(int track, char* soundSrc[]) {
	res = FMOD::System_Create(&sys);
	ErrCheck(res);

	res = sys->getVersion(&ver);
	ErrCheck(res);

	if (ver < FMOD_VERSION) {
		ErrPrint("FMOD lib version %08x doesn't match header version %08x", ver, FMOD_VERSION);
	}

	res = sys->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ErrCheck(res);

	sounds = new FMOD::Sound*[track];
	for (int i = 0; i < track; i++); {
		res = sys->createSound(SoundFileInclude(soundSrc[0]), FMOD_DEFAULT, 0, &sounds[0]);
		ErrCheck(res);
	}
}

void ErrCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		ErrPrint("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void ErrPrint(const char* format, ...) {
	char errMsg[ERR_MSG_LEN];

	va_list args;
	va_start(args, format);
	vsnprintf(errMsg, ERR_MSG_LEN, format, args);
	//TODO: 아래 코드를 삭제하고, 콘솔을 일관되게 업데이트 할 수 있는 함수를 구현할 것
	perror(errMsg);
	va_end(args);
	//errMsg[ERR_MSG_LEN - 1] = '\0';

	exit(0);
}

char* SoundFileInclude(const char* fileName) {
	char* filePath{ (char*)calloc(256, sizeof(char)) };
	const char* soundLoc{ "../source/sound/" };

	strcat(filePath, soundLoc);
	strcat(filePath, fileName);

	FILE* file;
	if (file = fopen(filePath, "r")) {
		fclose(file);
		soundPathList.push_back(filePath);
	}

	return filePath;
}