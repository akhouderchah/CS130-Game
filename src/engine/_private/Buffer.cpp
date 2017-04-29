//@TODO try to delete buf object
#include "Buffer.h"

Buffer::Buffer()
{
}


Buffer::~Buffer()
{
	//delete[] buf;
}

unsigned char* Buffer::returnBuffer()
{
	return buf;
}

unsigned int Buffer::returnDataSize()
{
	return dataSize;
}

ALuint Buffer::returnFrequency()
{
	return frequency;
}
ALuint Buffer::returnFormat()
{
	return format;
}

bool Buffer::returnIsLoop()
{
	return isLoop;
}


std::string Buffer::createBuffer(std::string filePath, bool isLoop)
{
	this->isLoop = isLoop;

	const char * filePathChar = filePath.c_str();

	//playSound local variables
	unsigned int chunkSize;
	short formatType, channels;
	unsigned int sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	

	
	char type[4];
	unsigned int size = 0;

	FILE *fp = NULL;
	fp = fopen(filePathChar, "rb");

	//checking to make sure the file is in correct format
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
	{
		return "no RIFF";
	}

	fread(&size, sizeof(unsigned int), 1, fp);
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
	{
		return "not WAVE";
	}

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
	{
		return "not fmt";
	}

	//reading and storing the info about the WAVE file
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(unsigned int), 1, fp);
	fread(&avgBytesPerSec, sizeof(unsigned int), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	//Making sure that we reach the data
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
	{
		return "Missing DATA";
	}

	fread(&dataSize, sizeof(unsigned int), 1, fp);

	//allocating memory for sound data and loading the data
	buf = new unsigned char[dataSize];
	fread(buf, sizeof(char), dataSize, fp);

	fclose(fp);

	//generating a source from which the sound is going to be played from
	//And a buffer that will contain the sound data
	frequency = sampleRate;
	format = 0;

	//letting OpenAL know the formate of the sound
	if (bitsPerSample == 8)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO8;
		}
		else if (channels == 2)
		{
			format = AL_FORMAT_STEREO8;
		}
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else if (channels == 2)
		{
			format = AL_FORMAT_STEREO16;
		}
	}

	return "OK";
}