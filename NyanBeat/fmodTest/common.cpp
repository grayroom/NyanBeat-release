#include "pch.h"
#include "common.h"

void errCheck(const FMOD_RESULT result) {
	if (result != FMOD_OK) {
		errPrint("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void errPrint(const char* format, ...) {
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