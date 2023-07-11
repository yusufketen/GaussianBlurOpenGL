#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& shaderName, const std::string vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void Destroy();

    void Bind();
    void Unbind();

    void Uniform1f(std::string name, float value);
    void Uniform1i(std::string name, int value);


private:
    bool Initialize();
    unsigned int CreateShader(const std::string& name, const std::string& source, unsigned int type);
    bool ReadFile(const std::string& filepath, std::string& output);

private:
    int programId;

    std::map<std::string, int> uniformMap;

    std::string name;
    std::string vsSource;
    std::string fsSource;
};
