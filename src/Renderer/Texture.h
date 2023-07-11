#pragma once

#include "Core.h"
#include <string>
#include "glad/glad.h"

class Texture
{
public:
	Texture(int width, int height);
	Texture(const std::string& path);
	~Texture();

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	uint32_t GetRendererID() const { return m_RendererID; }


	bool operator==(const Texture& other) const 
	{
		return m_RendererID == ((Texture&)other).m_RendererID;
	}
private:
	std::string m_Path;
	int m_Width, m_Height;
	uint32_t m_RendererID{};
	GLint m_InternalFormat, m_DataFormat;
};
