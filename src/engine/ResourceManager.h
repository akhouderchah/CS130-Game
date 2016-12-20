#pragma once

#include "Base.h"
#include <unordered_map>

enum class ShapeType : uint8_t { PLANE, CUBE };

/**
 * @brief Class to make sure resources are only loaded as needed
 */
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	//// Texture functions
	static GLuint LoadTexture(const std::string &str, bool hasAlpha=false);
	static bool UnloadTexture(const std::string &str);
	static void UnloadAllTextures();

	//// Model functions
	static std::pair<GLuint, GLuint> LoadShape(ShapeType type);

private:
	static std::unordered_map<std::string, GLuint> s_Textures;
	static std::unordered_map<std::string, std::pair<GLuint, GLuint>> s_Models;
};
