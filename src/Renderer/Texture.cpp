#include "Texture.h"
#include <stb_image.h>

// Color Buffer Texture Constructor
Texture::Texture(int width, int height)
	: m_Width(width), m_Height(height)
{
	m_InternalFormat = GL_RGB;
	m_DataFormat = GL_RGB;

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);

}

Texture::Texture(const std::string& path)
	: m_Path(path)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		m_Width = width;
		m_Height = height;
		m_Path = path;

		if (nrChannels == 3) {
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else {
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}
