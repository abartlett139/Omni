#include "WAVE.h"

class SoundEngine;


extern SoundEngine *soundEngine;


class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();

	bool Load(char *fileName, bool LOOP);

	int Initialize();

	void PlayMusic(int soundNdx);
	void ChangeMusicVolume();
	void StopMusic();
	void PlayFX(int soundNdx);

	IXAudio2 *engine;
	IXAudio2SourceVoice *musicSource, *fxSource;
	IXAudio2MasteringVoice *master;

	std::vector<Wave*> soundList;

	enum { MENU_BACKGROUND, GAME_BACKGROUND };

	float fxVolume, musicVolume;
};

