#include "pch.h"
#include "sound.h"

std::vector<char *> soundPathList{};

Sound::Sound() {}

Sound::Sound(int track, std::vector<fs::path> soundSrc) {
	res = FMOD::System_Create(&sys);
	errCheck(res);

	res = sys->getVersion(&ver);
	errCheck(res);

	if (ver < FMOD_VERSION) {
		errPrint("FMOD lib version %08x doesn't match header version %08x", ver, FMOD_VERSION);
	}

	res = sys->init(32, FMOD_INIT_NORMAL, extradriverdata);
	errCheck(res);

	res = sys->getMasterChannelGroup(&masterGroup);
	errCheck(res);

	char fileDir[PATH_LEN];

	std::cout << "Loading Sound Files... ";
	tracks.resize(soundSrc.size() < track ? soundSrc.size() : track);
	for (int i = 0; i < tracks.size(); ++i) {
		wcstombs(fileDir, soundSrc[i].c_str(), PATH_LEN);
		res = sys->createSound(fileDir, FMOD_DEFAULT, 0, &tracks[i].sound);
		errCheck(res);

		tracks[i].name = soundSrc[i].filename().string();
	}
	std::cout << "OK!" << std::endl;

	res = sys->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dspChorus);
	errCheck(res);

	res = masterGroup->addDSP(0, dspChorus);
	errCheck(res);

	res = dspChorus->setBypass(true);
	errCheck(res);
}

/*
 * Get Methods
 */
FMOD::System* Sound::getSys() {
	return this->sys;
}

std::vector<Sound::Track> Sound::getTracks() {
	return this->tracks;
}

Sound::Track Sound::getTrack(const int num) {
	return this->tracks[num];
}

FMOD::Channel* Sound::getChannel() {
	return this->channel;
}

FMOD::DSP* Sound::getDspEcho() {
	return dspChorus;
}

void Sound::PlaySoundNo(const int trackNum/*, const int channelNum = 0*/) {
	res = sys->playSound(tracks[trackNum].sound, 0, false, &channel);
	std::cout << "playing track no." << trackNum << std::endl;
	errCheck(res);
}

void Sound::ToggleDspEffect(FMOD::DSP* dsp) {
	bool bypassStat;

	res = dsp->getBypass(&bypassStat);
	errCheck(res);

	res = dsp->setBypass(!bypassStat);
	errCheck(res);
}