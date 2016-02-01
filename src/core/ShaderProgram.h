#pragma once

#include "../base/Base.h"
#include <vector>

class ShaderProgram
{
public:
	ShaderProgram(size_t expectedShaders = 2);
	~ShaderProgram();

	void AddShader(GLenum shaderType, std::string shaderPath);
	/** Compile and link shaders, then return the program object. Returns GL_FALSE on error. **/
	GLuint Compile();
private:
	std::string GetSource(std::string shaderPath);
	GLuint CompileShader(GLenum shaderType, std::string shaderSource);

	// Don't copy programs
	ShaderProgram(const ShaderProgram&);
	ShaderProgram& operator=(const ShaderProgram&);
private:
	std::vector<std::pair<GLenum,std::string>> m_Shaders;
};

