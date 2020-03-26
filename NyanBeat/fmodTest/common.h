#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#define ERR_MSG_LEN 1024

void ErrCheck(const FMOD_RESULT result);
void ErrPrint(const char* format, ...);