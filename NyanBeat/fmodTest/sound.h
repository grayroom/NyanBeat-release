#pragma once

#include <iostream>
#include <vector>
#include <fmod.hpp>

#include "resource.h"
#include "common.h"

class Sound {
private:
	struct Track {
		FMOD::Sound* sound;
		string name;
	};

	FMOD::System*		sys{ nullptr };
	vector<Track>		tracks{};
	FMOD::Channel*		channel{}; // single channel을 상정합니다.
	FMOD::ChannelGroup* masterGroup{};

	FMOD::DSP*		dspChorus{};


	FMOD_RESULT res;
	unsigned int ver;
	void* extradriverdata;

public:
	Sound();
	Sound(int track, vector<fs::path> soundSrc);

	FMOD::System*			getSys();
	vector<Sound::Track>	getTracks();
	Sound::Track			getTrack(const int num);
	FMOD::Channel*			getChannel();

	FMOD::DSP*				getDspEcho();

	void PlaySoundNo(const int trackNum/*, const int channelNum = 0*/);
	void ToggleDspEffect(FMOD::DSP* dsp);
};