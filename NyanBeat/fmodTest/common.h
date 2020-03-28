#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <process.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#define ERR_MSG_LEN 1024

struct KeySet {
	/*
	 	[--------EMPTY 22 FIELD--------][987654321]
		Only lower 9 bit has valid key value. When number key pressed, corresponding bit set high(1). In this way, you can
		get 9 key status information at the same time(9-key rollover)
	*/
	int numKey;
	/*
		Get single command key
	*/
	char cmdKey;
};

unsigned WINAPI listenKeyPress(void* arg);

void hideCursor();
void moveCursor(const int x, const int y);

void errCheck(const FMOD_RESULT result);
void errPrint(const char* format, ...);