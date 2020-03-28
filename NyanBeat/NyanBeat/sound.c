#include "sound.h"
Sound::Sound() {
	res = FMOD::System_Create(&sys);
	errCheck(res);
}

void errCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		errPrint("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void errPrint(const char* format, ...) {
	char errMsg[ERR_MSG_LEN];

	va_list args;
	va_start(args, format);
	vsnprintf(errMsg, ERR_MSG_LEN, format, args);
	va_end(args);
	errMsg[ERR_MSG_LEN - 1] = '\0';

	exit(0);
}