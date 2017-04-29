#include "Sound.h"
#include <iostream>
<<<<<<< HEAD
#include <vector>
=======
<<<<<<< HEAD
#include <vector>
=======
>>>>>>> origin/soundImplementation
>>>>>>> origin/soundImplementation

std::unordered_map<std::string, int> Sound::sourceToNameConnection;



Sound::Sound()
{
	SourcePos[0] = 0.0; SourcePos[1] = 0.0; SourcePos[2] = 0.0;
	SourceVel[0] = 0.0; SourceVel[1] = 0.0; SourceVel[2] = 0.0;
	ListenerPos[0] = 0.0; ListenerPos[1] = 0.0; ListenerPos[2] = 0.0;
	ListenerVel[0] = 0.0; ListenerVel[1] = 0.0; ListenerVel[2] = 0.0;
	ListenerOri[0] = 0.0; ListenerOri[1] = 0.0; ListenerOri[2] = -1.0;
	ListenerOri[3] = 0.0; ListenerOri[4] = 1.0; ListenerOri[5] = 0.0;

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
}


std::string Sound::InitializeSound(std::vector<SoundFileData> soundFileData)
{
	numberOfSounds = static_cast<int>(soundFileData.size());
	std::cout << "Sound initialization " << numberOfSounds << std::endl;

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


	int i = 0;
	for (std::vector<SoundFileData>::iterator it = soundFileData.begin(); it != soundFileData.end(); ++it)
	{
<<<<<<< HEAD
		sourceToNameConnection.emplace(it->name, i);

		alBufferData(buffers[i], it->format, it->buf, it->dataSize, it->frequency);

		//these values are set by the alListenerfv() and alSourcei/fv() functions And the source is associated with a buffer

		if (i == 0)
		{
			//listener
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
		}
=======
<<<<<<< HEAD
		sourceToNameConnection.emplace(it->name, i);

		alBufferData(buffers[i], it->format, it->buf, it->dataSize, it->frequency);

		//these values are set by the alListenerfv() and alSourcei/fv() functions And the source is associated with a buffer

		if (i == 0)
		{
			//listener
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
		}
=======
		alBufferData(buffers[i], bufferObjects[i].returnFormat(), bufferObjects[i].returnBuffer(), bufferObjects[i].returnDataSize(), bufferObjects[i].returnFrequency());
	}

	//these values are set by the alListenerfv() and alSourcei/fv() functions And the source is associated with a buffer

	//listener
	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
>>>>>>> origin/soundImplementation
>>>>>>> origin/soundImplementation

		//Source
		alSourcei(sources[i], AL_BUFFER, buffers[i]);
		alSourcei(sources[i], AL_PITCH, 1.0f);
		alSourcei(sources[i], AL_GAIN, 1.0f);
		alSourcei(sources[i], AL_BUFFER, buffers[i]);
		alSourcefv(sources[i], AL_POSITION, SourcePos);
		alSourcefv(sources[i], AL_VELOCITY, SourceVel);

		if (it->isLoop)
		{
			alSourcei(sources[i], AL_LOOPING, AL_TRUE);
		}
		else
		{
			alSourcei(sources[i], AL_LOOPING, AL_FALSE);
		}


		i++;
	}
	
	return "OK";
}


std::string Sound::play(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = sourceToNameConnection.find(name);

	if (iter == sourceToNameConnection.end())
	{
		return "Sound not found";
	}	
	else
	{
		alSourcePlay(sources[iter->second]);
		return "OK";
	}
}

std::string Sound::pause(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = sourceToNameConnection.find(name);
	if (iter == sourceToNameConnection.end())
	{
		return "Sound not found";
	}
	else
	{
		alSourcePause(sources[iter->second]);
		return "OK";
	}
}


std::string Sound::stop(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = sourceToNameConnection.find(name);
	if (iter == sourceToNameConnection.end())
	{
		return "Sound not found";
	}
	else
	{
		alSourceStop(sources[iter->second]);
		return "OK";
	}
}