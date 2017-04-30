#include "ResourceManager.h"
#include "DrawComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA
#include "stb_image.h"

// Static member variable initialization
std::unordered_map<std::string, GLuint> ResourceManager::s_Textures;
std::unordered_map<std::string, std::pair<GLuint, GLuint>> ResourceManager::s_Models;


using namespace glm;
typedef PackageFormat::TextureHeader TextureHeader;

ResourceManager::ResourceManager()
{
	
}

ResourceManager::~ResourceManager()
{

}

//Added by Hovhannes
SoundFileData ResourceManager::LoadSound(const std::string &name, const std::string &str, const bool isLoop)
{
	SoundFileData soundFileData;

	soundFileData.name = name;
	soundFileData.isLoop = isLoop;

	const char * filePathChar = str.c_str();

	//playSound local variables
	unsigned int chunkSize;
	short formatType, channels;
	unsigned int sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;

	char type[4];
	unsigned int size = 0;

	if (FILE *fp = fopen(filePathChar, "rb"))
	{
		//checking to make sure the file is in correct format
		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
		{
			soundFileData.errorCode = "No RIFF";
			return soundFileData;
		}

		fread(&size, sizeof(unsigned int), 1, fp);
		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
		{
			soundFileData.errorCode = "Not WAVE";
			return soundFileData;
		}

		fread(type, sizeof(char), 4, fp);
		if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
		{
			soundFileData.errorCode = "Not fmt ";
			return soundFileData;
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
			soundFileData.errorCode = "No data";
			return soundFileData;
		}

		fread(&soundFileData.dataSize, sizeof(unsigned int), 1, fp);

		//allocating memory for sound data and loading the data
		soundFileData.buf = new unsigned char[soundFileData.dataSize];
		fread(soundFileData.buf, sizeof(char), soundFileData.dataSize, fp);

		fclose(fp);

		//generating a source from which the sound is going to be played from
		//And a buffer that will contain the sound data
		soundFileData.frequency = sampleRate;
		soundFileData.format = 0;

		//letting OpenAL know the formate of the sound
		if (bitsPerSample == 8)
		{
			if (channels == 1)
			{
				soundFileData.format = AL_FORMAT_MONO8;
			}
			else if (channels == 2)
			{
				soundFileData.format = AL_FORMAT_STEREO8;
			}
		}
		else if (bitsPerSample == 16)
		{
			if (channels == 1)
			{
				soundFileData.format = AL_FORMAT_MONO16;
			}
			else if (channels == 2)
			{
				soundFileData.format = AL_FORMAT_STEREO16;
			}
		}

		soundFileData.errorCode = "OK";
		return soundFileData;

	}
	else
	{
		soundFileData.errorCode = "Could not open file " + name;
		return soundFileData;
	}
}



GLuint ResourceManager::LoadTexture(const std::string &str, TextureType type)
{
	auto iter = s_Textures.find(str);

	// Shortcut if we already loaded the texture
	if(iter != s_Textures.end())
	{
		return iter->second;
	}

	// Actually load the texture
	int comp, h, w;
	unsigned char *pImage;
	GLuint tex = 0;

	if(type == TextureType::RGBA)
	{
		pImage = stbi_load(str.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	}
	else
	{
		pImage = stbi_load(str.c_str(), &w, &h, &comp, STBI_rgb);
	}

	if(pImage == nullptr)
	{
		// TODO log error
		goto error;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if(comp == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	}
	else if(comp == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
	}
	else
	{
		// TODO log error
		goto error;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImage);
	s_Textures[str] = tex;
	return tex;

error:
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImage);
	if(tex)
	{
		glDeleteTextures(1, &tex);
	}
	return 0;
}

/*
GLuint ResourceManager::LoadTexture(const std::string &name, TextureHeader *pHdr)
{
	auto iter = s_Textures.find(str);

	// Shortcut if we already loaded the texture
	if(iter != s_Textures.end())
	{
		return iter->second;
	}

	// Actually load the texture
	int comp, h, w;
	unsigned char *pImage;
	GLuint tex = 0;

	if(pHdr->Flags & TextureHeader::HAS_ALPHA)
	{
		pImage = stbi_load(str.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	}
	else
	{
		pImage = stbi_load(str.c_str(), &w, &h, &comp, STBI_rgb);
	}

	if(pImage == nullptr)
	{
		// TODO log error
		goto error;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if(comp == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	}
	else if(comp == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
	}
	else
	{
		// TODO log error
		goto error;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImage);
	s_Textures[str] = tex;
	return tex;

error:
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImage);
	if(tex)
	{
		glDeleteTextures(1, &tex);
	}
	return 0;
}
*/

bool ResourceManager::UnloadTexture(const std::string &str)
{
	auto iter = s_Textures.find(str);
	if(iter != s_Textures.end())
	{
		glDeleteTextures(1, &(iter->second));
		s_Textures.erase(iter);
		return true;
	}

	return false;
}

void ResourceManager::UnloadAllTextures()
{
	for(auto tex : s_Textures)
	{
		glDeleteTextures(1, &(tex.second));
	}

	s_Textures.clear();
}

std::pair<GLuint, GLuint> ResourceManager::LoadShape(ShapeType type)
{
	static bool isPlaneLoaded = false;
	static bool isCubeLoaded = false;

	switch(type)
	{
	case ShapeType::PLANE:
		if(isPlaneLoaded)
		{
			return s_Models["PLANE"];
		}
		else
		{
			DrawComponent::Vertex vertices[] = {
				{ vec3(-1.f, -1.f, 0.f), vec2(1.f, 1.f) },
				{ vec3(1.f, -1.f, 0.f), vec2(0.f, 1.f) },
				{ vec3(1.f, 1.f, 0.f), vec2(0.f, 0.f) },
				{ vec3(-1.f, 1.f, 0.f), vec2(1.f, 0.f) }
			};

			unsigned int indices [] = { 2, 1, 0,
										0, 3, 2 };

			// TODO Error handling; also move most code out of switch case!
			GLuint VBO, IBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE,
								  sizeof(DrawComponent::Vertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,
								  sizeof(DrawComponent::Vertex),
								  (const GLvoid*)sizeof(glm::vec3));

			return std::make_pair(VBO, IBO);
		}
	case ShapeType::CUBE:
		if(isCubeLoaded)
		{
			return s_Models["CUBE"];
		}
		else
		{
			// TODO
			return std::make_pair(0, 0);
		}
	default:
		return std::make_pair(0, 0);
	}
}