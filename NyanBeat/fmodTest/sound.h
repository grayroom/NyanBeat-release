#pragma once

#include <iostream>
#include <vector>
#include <fmod.hpp>

#include "resource.h"
#include "common.h"

class Sound {
private:
	FMOD::System* sys{ nullptr };
	FMOD::Sound** sounds;
	FMOD::Channel* channel; // single channel을 상정합니다.

	FMOD_RESULT res;
	unsigned int ver;
	void* extradriverdata;

public:
	Sound();
	Sound(int track, vector<fs::path> soundSrc);

	FMOD::System* getSys();
	FMOD::Sound** getSounds();
	FMOD::Sound* getSound(const int num);
	FMOD::Channel* getChannel();

	void PlaySoundNo(const int soundNum/*, const int channelNum = 0*/);
};