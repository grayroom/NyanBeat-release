#pragma once

#define ERR_MSG_LEN 1024


void errCheck(const FMOD_RESULT result);
void errPrint(const char* format, ...);