#include "ShaderProgram.h"
#include <fstream>

using namespace std;

ShaderProgram::ShaderProgram(size_t expectedShaders)
{
	m_Shaders.reserve(expectedShaders);
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::AddShader(GLenum shaderType, string shaderPath)
{
	m_Shaders.push_back(make_pair(shaderType, shaderPath));
}

GLuint ShaderProgram::Compile()
{
	GLuint program = glCreateProgram();
	if(!program)
	{
		// @TODO - LOG ERROR
		glDeleteProgram(program);
		return GL_NONE;
	}

	vector<GLuint> shaders;
	shaders.resize(m_Shaders.size());
	for(size_t i = 0; i < shaders.size(); ++i)
	{
		shaders[i] = CompileShader(m_Shaders[i].first, GetSource(m_Shaders[i].second));
		glAttachShader(program, shaders[i]);
	}

	// Link Program
	GLint success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
		goto error;

	// Validate Program
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if(!success)
		goto error;

	// Cleanup
	for(size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	return program;

error:
	GLchar errorLog[1024];
	glGetProgramInfoLog(program, 1024, NULL, errorLog);
	// @TODO - LOG ERROR
	glDeleteProgram(program);
	return GL_NONE;
}

string ShaderProgram::GetSource(string shaderPath)
{
	ifstream shaderFile(shaderPath, ios::in | ios::binary);

	if(shaderFile)
	{
		string shaderString;
		shaderFile.seekg(0, ios::end);
		shaderString.resize((size_t)shaderFile.tellg());

		shaderFile.seekg(0, ios::beg);
		shaderFile.read(&shaderString[0], shaderString.size());
		shaderFile.close();

		return shaderString;
	}

	// @TODO - LOG ERROR, DON'T JUST RETURN!!!
	return "";
}

GLuint ShaderProgram::CompileShader(GLenum shaderType, string shaderSource)
{
	GLuint shaderObj = glCreateShader(shaderType);
	if(!shaderObj)
	{
		// @TODO - LOG ERROR
		glDeleteShader(shaderObj);
		return GL_NONE;
	}

	const GLchar* shader = shaderSource.c_str();
	GLint length = shaderSource.size();
	
	glShaderSource(shaderObj, 1, &shader, &length);

	GLint success;
	glCompileShader(shaderObj);
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLchar errorLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, errorLog);
		// @TODO - LOG ERROR
		glDeleteShader(shaderObj);
		return GL_NONE;
	}

	return shaderObj;
}

