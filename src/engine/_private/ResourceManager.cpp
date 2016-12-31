#include "ResourceManager.h"
#include "DrawComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA
#include "stb_image.h"

// Static member variable initialization
std::unordered_map<std::string, GLuint> ResourceManager::s_Textures;
std::unordered_map<std::string, std::pair<GLuint, GLuint>> ResourceManager::s_Models;

using namespace glm;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
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
