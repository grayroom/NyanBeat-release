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

	track = soundSrc.size() < track ? soundSrc.size() : track;
	sounds = new FMOD::Sound*[track];
	char fileDir[PATH_LEN];
	
	cout << "Loading Sound Files... ";
	
	for (int i = 0; i < track; i++) {
		wcstombs(fileDir, soundSrc[i].c_str(), PATH_LEN);
		res = sys->createSound(fileDir, FMOD_DEFAULT, 0, &sounds[i]);
		ErrCheck(res);
	}
	cout << "OK!" << endl;
	cout << track << " tracks have been loaded" << endl;
}

/*
 * Get Methods
 */
FMOD::System* Sound::getSys() {
	return this->sys;
}

FMOD::Sound** Sound::getSounds() {
	return this->sounds;
}

FMOD::Sound* Sound::getSound(const int num) {
	return this->sounds[num];
}

FMOD::Channel* Sound::getChannel() {
	return this->channel;
}

void Sound::PlaySoundNo(const int soundNum/*, const int channelNum = 0*/) {
	res = sys->playSound(sounds[soundNum], 0, false, &channel);
	ErrCheck(res);
}
