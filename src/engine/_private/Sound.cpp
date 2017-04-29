#include "Sound.h"
#include <iostream>


std::unordered_map<std::string, int> Sound::sourceToNameConnection;

Sound::Sound()
{
	numberOfSounds = 0;
}


Sound::~Sound()
{
	/*
	alDeleteSources(numberOfSounds, sources);
	alDeleteBuffers(numberOfSounds, buffers);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	*/
	std::cout << " In Destroyer " << std::endl;
}

std::string Sound::createSoundBuffers(std::string name, std::string filePath, bool isLoop)
{
	std::string returnValue = bufferObjects[numberOfSounds].createBuffer(filePath, isLoop);
	if (returnValue == "OK")
	{
		sourceToNameConnection.emplace(name, numberOfSounds);
		numberOfSounds += 1;

		return returnValue;
	}
	else
	{
		return returnValue;
	}
	
}

std::string Sound::InitializeSound()
{

	//So we loaded the file, now we initialize the OpenAl
	device = alcOpenDevice(NULL);
	if (!device)
	{
		return "No sound device";
	}
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context)
	{
		return "No sound context";
	}

	alGenBuffers(numberOfSounds, buffers);
	alGenSources(numberOfSounds, sources);


	for (int i = 0; i < numberOfSounds; i++)
	{
		alBufferData(buffers[i], bufferObjects[i].returnFormat(), bufferObjects[i].returnBuffer(), bufferObjects[i].returnDataSize(), bufferObjects[i].returnFrequency());
	}





	//these values are set by the alListenerfv() and alSourcei/fv() functions And the source is associated with a buffer


	//listener
	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);

	for (int i = 0; i < numberOfSounds; i++)
	{
		//Source
		alSourcei(sources[i], AL_BUFFER, buffers[i]);
		alSourcei(sources[i], AL_PITCH, 1.0f);
		alSourcei(sources[i], AL_GAIN, 1.0f);
		alSourcei(sources[i], AL_BUFFER, buffers[i]);
		alSourcefv(sources[i], AL_POSITION, SourcePos);
		alSourcefv(sources[i], AL_VELOCITY, SourceVel);

		if (bufferObjects[i].returnIsLoop())
		{
			alSourcei(sources[i], AL_LOOPING, AL_TRUE);
		}
		else
		{
			alSourcei(sources[i], AL_LOOPING, AL_FALSE);
		}
	}
	return " ";
}


void Sound::play(std::string name)
{
	alSourcePlay(sources[sourceToNameConnection[name]]);
}

void Sound::pause(std::string name)
{
	alSourcePause(sources[sourceToNameConnection[name]]);
}


void Sound::stop(std::string name)
{
	alSourceStop(sources[sourceToNameConnection[name]]);
}