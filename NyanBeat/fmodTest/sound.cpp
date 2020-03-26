#include "sound.h"

std::vector<char *> soundPathList{};

Sound::Sound() {}

Sound::Sound(int track, vector<fs::path> soundSrc) {
	res = FMOD::System_Create(&sys);
	ErrCheck(res);

	res = sys->getVersion(&ver);
	ErrCheck(res);

	if (ver < FMOD_VERSION) {
		ErrPrint("FMOD lib version %08x doesn't match header version %08x", ver, FMOD_VERSION);
	}

	res = sys->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ErrCheck(res);

	res = sys->getMasterChannelGroup(&masterGroup);
	ErrCheck(res);

	char fileDir[PATH_LEN];
	cout << "Loading Sound Files... ";
	tracks.resize(soundSrc.size() < track ? soundSrc.size() : track);
	for (int i = 0; i < tracks.size(); i++) {
		wcstombs(fileDir, soundSrc[i].c_str(), PATH_LEN);
		res = sys->createSound(fileDir, FMOD_DEFAULT, 0, &tracks[i].sound);
		ErrCheck(res);

		tracks[i].name = soundSrc[i].filename().string();
	}
	cout << "OK!" << endl;

	res = sys->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dspChorus);
	ErrCheck(res);

	res = masterGroup->addDSP(0, dspChorus);
	ErrCheck(res);

	res = dspChorus->setBypass(true);
	ErrCheck(res);
}

/*
 * Get Methods
 */
FMOD::System* Sound::getSys() {
	return this->sys;
}

vector<Sound::Track> Sound::getTracks() {
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
	cout << "playing track no." << trackNum << endl;
	ErrCheck(res);
}

void Sound::ToggleDspEffect(FMOD::DSP* dsp) {
	bool bypassStat;

	res = dsp->getBypass(&bypassStat);
	ErrCheck(res);

	res = dsp->setBypass(!bypassStat);
	ErrCheck(res);
}