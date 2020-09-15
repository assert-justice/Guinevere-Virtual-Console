#include "Audio.h"
#include <stdlib.h>
#include <iostream>
#define NULL 0
#define SAMPLE_SIZE 48000
Audio::Audio()
{
	
}

bool Audio::hasError()
{
	ALenum error = alGetError();
	if (error == AL_NO_ERROR) return false;

	switch (error)
	{
	case AL_INVALID_NAME:
		std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
		break;
	case AL_INVALID_ENUM:
		std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
		break;
	case AL_INVALID_VALUE:
		std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
		break;
	case AL_INVALID_OPERATION:
		std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
		break;
	case AL_OUT_OF_MEMORY:
		std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
		break;
	default:
		std::cerr << "UNKNOWN AL ERROR: " << error;
	}
	std::cerr << std::endl;

	return true;
}

void Audio::init()
{

	device = alcOpenDevice(NULL);
	if (hasError()) return;

	context = alcCreateContext(device, NULL);
	if (hasError()) return;

	alcMakeContextCurrent(context);

	alGenBuffers((ALsizei)4, buffers);
	if (hasError()) return;

	ALsizei size = 65536;
	data[0] = (char*)malloc(size);
	data[1] = (char*)malloc(size);
	data[2] = (char*)malloc(size);
	data[3] = (char*)malloc(size);
	for (size_t i = 0; i < size; i++) // triangle
	{
		data[0][i] = i % 256 - 128;
	}
	for (size_t i = 0; i < size; i++) // triangle
	{
		data[1][i] = i % 256 - 128;
	}
	for (size_t i = 0; i < size; i++) // square
	{
		//data[2][i] = i % 256 < 128 ? -127 : 127;
		data[2][i] = -127;
		if (i % 256 < 128) data[2][i] = 127;
	}
	for (size_t i = 0; i < size; i++)
	{
		data[3][i] = rand() % 256 - 127;
	}

	alGenSources(4, sources);

	for (size_t i = 0; i < 4; i++)
	{
		alBufferData(buffers[i], AL_FORMAT_STEREO16, data[i], size, 48000);
		alSourcef(sources[i], AL_PITCH, 1);
		alSourcef(sources[i], AL_GAIN, 0);
		alSource3f(sources[i], AL_POSITION, 0, 0, 0);
		alSource3f(sources[i], AL_VELOCITY, 0, 0, 0);
		alSourcei(sources[i], AL_LOOPING, AL_TRUE);
		alSourcei(sources[i], AL_BUFFER, buffers[i]);
		alSourcePlay(sources[i]);
	}
}

void Audio::setChannel(int channel, ALfloat gain, ALfloat pitch)
{
	alSourcef(sources[channel], AL_GAIN, gain);
	alSourcef(sources[channel], AL_PITCH, pitch);
}

void Audio::update()
{
	
}

Audio::~Audio()
{
	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data[3]);
	alDeleteSources(4, sources);
	alDeleteBuffers(4, buffers);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}