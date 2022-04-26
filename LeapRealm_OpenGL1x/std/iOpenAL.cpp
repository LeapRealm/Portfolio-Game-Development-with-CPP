#include "iOpenAL.h"

#include "iStd.h"

iOpenAL::iOpenAL(int audioNum)
{
	bufCount = audioNum;
	gBuffer = new ALuint[audioNum];
	gSource = new ALuint[audioNum];

	ALCdevice* newDevice = alcOpenDevice(nullptr);
	if (newDevice)
	{
		ALCint attr[] = {ALC_FREQUENCY, 22050, 0};
		ALCcontext* newContext = alcCreateContext(newDevice, attr);
		if (newContext)
		{
			alcMakeContextCurrent(newContext);

			alGenBuffers(bufCount, gBuffer);
			ALenum error = alGetError();
			if (error != AL_NO_ERROR)
			{
				printf("error : generating buffer\n");
				return;
			}

			alGenSources(bufCount, gSource);
			error = alGetError();
			if (error != AL_NO_ERROR)
			{
				printf("error : generating source\n");
				return;
			}
		}
	}

	// clear any error
	alGetError();
}

iOpenAL::~iOpenAL()
{
#if 0
	alDeleteSources(bufCount, gSource);
	alDeleteBuffers(bufCount, gBuffer);
#else
	ALCuint* names = new ALCuint(bufCount);
	alDeleteSources(bufCount, names);
	alDeleteBuffers(bufCount, names);
	delete names;
#endif

	ALCcontext* context = alcGetCurrentContext();
	ALCdevice* device = alcGetContextsDevice(context);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

struct WaveHeader0
{
	char riff[4];					// Chunk ID : "RIFF"
	unsigned int riffSize;			// Chunk size : 4 + n
	char wave[4];					// Wave ID : "WAVE"

	char fmt[4];					// Chunk ID : "fmt"
	unsigned int fmtSize;			// Chunk size : 16 or 18 or 40
	unsigned short format;			// Format code
	unsigned short channels;		// Number of interleaved channel
	unsigned int samplesPerSec;		// Sampling rate (blocks per second)
	unsigned int bytesPerSec;		// Date rate
	unsigned short blockAlign;		// Data block size(bytes)
	unsigned short bitsPerSample;	// Bits per sample
};

struct WaveHeader1
{
	char data[4];					// 'data'
	unsigned int dataSize;
};

void iOpenAL::initBuffer(int idx, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	int bufLen;
	char* buf = loadFile(szText, bufLen);
	WaveHeader0* head0 = (WaveHeader0*)buf;
	WaveHeader1* head1 = (WaveHeader1*)&buf[sizeof(WaveHeader0) + (head0->fmtSize - 16)];

	char* data = &buf[sizeof(WaveHeader0) + (head0->fmtSize - 16) + sizeof(WaveHeader1)];
	int dataLength = head1->dataSize;

	ALuint buffer = 0;
	ALuint format = 0;
	switch (head0->bitsPerSample)
	{
	case 8:  format = (head0->channels==1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8); break;
	case 16: format = (head0->channels==1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16); break;
	default: break;
	}

	ALsizei samplesPerSec = head0->samplesPerSec;

	alBufferData(gBuffer[idx], format, data, dataLength, samplesPerSec);

	delete buf;

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error attaching audio to buffer : [%s] %x\n", szText, error);
	}
}

void iOpenAL::initSource(int idx, bool isRepeat)
{
	ALenum error = AL_NO_ERROR;
	// clear the error
	alGetError();

	alSourcei(gSource[idx], AL_LOOPING, isRepeat);

	const float sourcePosAL[] = { 0.0f, 0.0f, 0.0f }; // x, y, distance
	alSourcefv(gSource[idx], AL_POSITION, sourcePosAL);

	alSourcef(gSource[idx], AL_REFERENCE_DISTANCE, 50.0f);

	alSourcei(gSource[idx], AL_BUFFER, gBuffer[idx]);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error attaching buffer to source : %x\n", error);
	}
}

void iOpenAL::play(int idx)
{
	alSourcePlay(gSource[idx]);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error starting source : %x\n", error);
	}
}

void iOpenAL::play(int idx, bool isRepeat)
{
	alSourcei(gSource[idx], AL_LOOPING, isRepeat);
	play(idx);
}

void iOpenAL::pause(int idx)
{
	alSourcePause(gSource[idx]);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error pause source : %x\n", error);
	}
}

void iOpenAL::stop(int idx)
{
	alSourceStop(gSource[idx]);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error stop source : %x\n", error);
	}
}

void iOpenAL::volume(int idx, float vol)
{
	alSourcef(gSource[idx], AL_GAIN, vol);

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("error volume source : %x\n", error);
	}
}

static iOpenAL* al;

AudioInfo* audioInfo;
int prevBgm;

void loadAudio(AudioInfo* ai, int aiNum)
{
	al = new iOpenAL(aiNum);

	audioInfo = new AudioInfo[aiNum];
	for (int i = 0; i < aiNum; i++)
	{
		al->initBuffer(i, ai[i].fileName);
		al->initSource(i, ai[i].isRepeat);

		AudioInfo* info = &audioInfo[i];
		char* s = new char[strlen(ai[i].fileName) + 1];
		strcpy(s, ai[i].fileName);
		info->fileName = s;
		info->isRepeat = ai[i].isRepeat;
		info->volume = ai[i].volume;
	}
	prevBgm = -1;
}

void freeAudio()
{
	for (int i = 0; i < al->bufCount; i++)
		delete audioInfo[i].fileName;
	delete audioInfo;
	delete al;
}

void pauseAudio()
{
	for (int i = 0; i < al->bufCount; i++)
	{
		if (audioInfo[i].isRepeat)
			al->pause(i);
	}
}

void resumeAudio()
{
	if (prevBgm != -1)
		audioPlay(prevBgm);
}

void audioPlay(int index)
{
	al->play(index);

	if (audioInfo[index].isRepeat)
		prevBgm = index;
}

void audioStop()
{
	for (int i = 0; i < al->bufCount; i++)
	{
		if (audioInfo[i].isRepeat)
			al->stop(i);
	}
	prevBgm = -1;
}

void audioStop(int index)
{
	al->stop(index);
}

void audioVolume(float bgm, float eff, int effNum)
{
	if (eff < 0.1) eff = 0; else if (eff > 0.9) eff = 1;
	if (bgm < 0.1) bgm = 0; else if (bgm > 0.9) bgm = 1;

	for (int i = 0; i < effNum; i++)
		al->volume(i, eff * audioInfo[i].volume);

	for (int i = effNum; i < al->bufCount; i++)
		al->volume(i, bgm * audioInfo[i].volume);
}
