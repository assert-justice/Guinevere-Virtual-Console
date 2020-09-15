#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <cstdint>

class Audio
{
	ALuint  buffers[4];
	ALuint  sources[4];
	ALint source_state;

	ALsizei size, freq;
	ALenum  format;
	char* data[4];
	ALCcontext* context;
	ALCdevice* device;
	bool hasError();
public:
	Audio();
	~Audio();
	void init();
	void update();
	void setChannel(int channel, ALfloat gain, ALfloat pitch);
};

