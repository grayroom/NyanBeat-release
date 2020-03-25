#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>

#define ERR_MSG_LEN 1024

class Sound {
private:
	FMOD::System* sys{ nullptr };
	FMOD::Sound** sounds;
	FMOD::Channel* channel;
	
	FMOD_RESULT res;
	unsigned int ver;
	void* extradriverdata;

public:
	Sound();
	Sound(int track, char* soundSrc[]);
};

void ErrCheck(FMOD_RESULT result);
void ErrPrint(const char* format, ...);

char* SoundFileInclude(const char* fileName);
