#include "sound.h"
Sound::Sound() {
	res = FMOD::System_Create(&sys);
	ErrCheck(res);
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
	va_end(args);
	errMsg[ERR_MSG_LEN - 1] = '\0';

	exit(0);
}