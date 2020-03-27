#include "common.h"

void listenKeyPress() {
}

unsigned __stdcall listenKeyPress(void* arg) {


	std::cout << "GOTCHA!" << std::endl;

	return 0;
}

void ErrCheck(const FMOD_RESULT result) {
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