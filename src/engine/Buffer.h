#pragma once

#include <al.h>
#include <alc.h>
#include <string>
#include "ErrorSystem.h"


class Buffer
{
public:
	Buffer();
	~Buffer();
	std::string createBuffer(std::string, bool);
	unsigned char* returnBuffer();
	unsigned int returnDataSize();
	ALuint returnFrequency();
	ALuint returnFormat();
	bool returnIsLoop();

private:
	
	unsigned int dataSize;
	ALuint frequency;
	ALenum format;
	unsigned char* buf;
	bool isLoop;
	
};

