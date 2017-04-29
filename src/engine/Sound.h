#pragma once

#include <unordered_map>
#include "Buffer.h"

class Sound
{
public:
	Sound();
	~Sound();
	void play(std::string);
	void stop(std::string);
	void pause(std::string);
	std::string createSoundBuffers(std::string, std::string, bool);
	std::string InitializeSound();


private:
	//OpenAl needs to know to play the sound is the position and velocity of the
	//source and the listener
	//orientation of the listener is described by two vectors, one pointing in the direction the listener
	//looks at and a second pointing up

	
	ALfloat SourcePos[3];
	ALfloat SourceVel[3];
	ALfloat ListenerPos[3];
	ALfloat ListenerVel[3];
	ALfloat ListenerOri[6];

	ALCcontext *context;
	ALCdevice *device;

	ALuint buffers[32];
	ALuint sources[32];

	Buffer bufferObjects[32];
	int numberOfSounds;
	static std::unordered_map<std::string, int> sourceToNameConnection;
};