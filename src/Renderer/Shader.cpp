#include "Shader.h"

#include <streambuf>
#include <fstream>
#include <sstream>
#include <string>

#include "Core.h"
#include "glad/glad.h"

Shader::Shader(const std::string& shaderName, const std::string vertexShaderPath, const std::string& fragmentShaderPath)
    : name{ shaderName }
{
    if (!ReadFile(vertexShaderPath, vsSource))
    {
        fprintf(stderr, "Shader Error: Failed to read vertex shader file '%s'\n", vertexShaderPath.c_str());
        return;
    }

    if (!ReadFile(fragmentShaderPath, fsSource))
    {
        fprintf(stderr, "Shader Error: Failed to read fragment shader file '%s'\n", fragmentShaderPath.c_str());
        return;
    }

    Initialize();
}

Shader::~Shader()
{
    if (programId != -1)
        Destroy();
}

bool Shader::Initialize()
{
    GL(programId = glCreateProgram());

    unsigned int vertexId = CreateShader(name.c_str(), vsSource.c_str(), GL_VERTEX_SHADER);
    if (vertexId == -1)
        return false;

    unsigned int fragmentId = CreateShader(name.c_str(), fsSource.c_str(), GL_FRAGMENT_SHADER);
    if (fragmentId == -1)
        return false;

    GL(glAttachShader(programId, vertexId));
    GL(glAttachShader(programId, fragmentId));
    GL(glLinkProgram(programId));

    int success;
    char infoLog[512];

    GL(glGetProgramiv(programId, GL_LINK_STATUS, &success));
    if (!success)
    {
        GL(glGetShaderInfoLog(programId, 512, NULL, infoLog));
        fprintf(stderr, "Shader Error: Failed to link shader program '%s': \n%s\n", name.c_str(), infoLog);
        return false;
    }

    GL(glDeleteShader(vertexId));
    GL(glDeleteShader(fragmentId));

    int uniformCount;
    GL(glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount));

    int length, size;
    GLenum type;
    for (int i = 0; i < uniformCount; i++)
    {
        char name[64];
        GL(glGetActiveUniform(programId, (GLuint)i, 64, &length, &size, &type, name));

        GL(unsigned int uniformLocation = glGetUniformLocation(programId, name));

        uniformMap.insert(std::pair<const char *, unsigned int>(name, uniformLocation));
    }

    return true;
}

void Shader::Destroy()
{
    Unbind();
    GL(glDeleteProgram(programId));
    programId = -1;
}

void Shader::Bind()
{
    GL(glUseProgram(programId));
}

void Shader::Unbind()
{
    GL(glUseProgram(0));
}

unsigned int Shader::CreateShader(const std::string& name, const std::string& source, unsigned int type)
{
    unsigned int id;

    GL(id = glCreateShader(type));
    const GLchar* glSource{ source.c_str() };
    GL(glShaderSource(id, 1, &glSource, NULL));
    GL(glCompileShader(id));

    char infoLog[512];

    int success;
    GL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GL(glGetShaderInfoLog(id, 512, NULL, infoLog));
        fprintf(stderr, "Shader Error: Failed to compile shader '%s': \n%s\n", name.c_str(), infoLog);
        return -1;
    }

    return id;
}

bool Shader::ReadFile(const std::string& filepath, std::string& output)
{
	std::ifstream stream;
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
		stream.open(filepath);
		std::stringstream stringStream;
		stringStream << stream.rdbuf();
		stream.close();
		output = stringStream.str();
	}
    catch (std::ifstream::failure e)
    {
		if (stream.is_open())
			stream.close();
		return false;
	}
	return true;
}

void Shader::Uniform1f(std::string name, float value)
{
    unsigned int location = uniformMap[name];
    GL(glUniform1f(location, value));
}

void Shader::Uniform1i(std::string name, int value)
{
    unsigned int location = uniformMap[name];
    GL(glUniform1i(location, value));
}
