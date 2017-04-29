#pragma once

#include "Base.h"
#include "Package.h"
#include <unordered_map>

#include "Sound.h"

enum class ShapeType : uint8_t { PLANE, CUBE };

enum class TextureType : uint8_t { RGB, RGBA };

/**
 * @brief Class to make sure resources are only loaded as needed
 */
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	//// Texture functions
	// @TODO remove non-package version
	static GLuint LoadTexture(const std::string &str, TextureType type);
	//static GLuint LoadTexture(const std::string &name, PackageFormat::TextureHeader *pHdr);
	static bool UnloadTexture(const std::string &str);
	static void UnloadAllTextures();

	//// Model functions
	static std::pair<GLuint, GLuint> LoadShape(ShapeType type);

	//Added by Hovhannes
	////Sound functions
	//@TODO add sound file type if possible
	static void LoadSound(const std::string &name, const std::string &str, const bool);
	static Sound * initializeSound();
	static Sound * returnSound();

private:
	static std::unordered_map<std::string, GLuint> s_Textures;
	static std::unordered_map<std::string, std::pair<GLuint, GLuint>> s_Models;

	static Sound sound;
};
