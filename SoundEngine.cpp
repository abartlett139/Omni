#include "SoundEngine.h"


SoundEngine::SoundEngine()
{
}


SoundEngine::~SoundEngine()
{
	engine->Release();
}

bool SoundEngine::Load(char * fileName, bool LOOP)
{
	Wave *tempSound = new Wave();
	if (LOOP) {
		tempSound->setToLoop();
	}
	if (!tempSound->load(fileName)) {
		engine->Release();
		CoUninitialize();
		return false;
	}
	else {
		soundList.push_back(tempSound);
		return true;
	}
}

int SoundEngine::Initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&engine))) {
		CoUninitialize();
		return -1;
	}

	if (FAILED(engine->CreateMasteringVoice(&master))) {
		engine->Release();
		CoUninitialize();
		return -1;
	}

	musicVolume = .5f;
	fxVolume = .5f;

	Load("audio/Some-Dreamy-Place.wav", true);
	Load("audio/Trouble-in-the-Kingdom.wav", true);
}

void SoundEngine::PlayMusic(int soundNdx)
{
	engine->CreateSourceVoice(&musicSource, soundList[soundNdx]->wf());
	musicSource->SetVolume(musicVolume, XAUDIO2_COMMIT_ALL);


	musicSource->Start();
	musicSource->SubmitSourceBuffer(soundList[soundNdx]->xaBuffer());
}

void SoundEngine::ChangeMusicVolume()
{
	musicSource->SetVolume(musicVolume, XAUDIO2_COMMIT_ALL);
}


void SoundEngine::StopMusic() {
	musicSource->Stop();
}


void SoundEngine::PlayFX(int soundNdx) {
	engine->CreateSourceVoice(&fxSource, soundList[soundNdx]->wf());

	fxSource->SetVolume(fxVolume, XAUDIO2_COMMIT_ALL);
	fxSource->Start();
	fxSource->SubmitSourceBuffer(soundList[soundNdx]->xaBuffer());
}