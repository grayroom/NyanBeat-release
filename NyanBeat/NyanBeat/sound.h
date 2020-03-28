#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#define ERR_MSG_LEN 1024

class Sound {
private:
	FMOD::System* sys;
	FMOD::Sound** sounds;
	FMOD::Channel* channel;
	
	FMOD_RESULT res;
public:
	Sound();
};

void errCheck(FMOD_RESULT result);
void errPrint(const char* format, ...);