#pragma once

#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

class iOpenAL
{
public:
	iOpenAL(int audioNum);
	virtual ~iOpenAL();

	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool isRepeat);

	void play(int idx);
	void play(int idx, bool isRepeat);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol);

public:
	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;
};

struct AudioInfo
{
	char* fileName;
	bool isRepeat;
	float volume;
};

void loadAudio(AudioInfo* ai, int aiNum);
void freeAudio();
void pauseAudio();
void resumeAudio();
void audioPlay(int index);
void audioStop();
void audioStop(int index);
void audioVolume(float bgm, float eff, int effNum);
