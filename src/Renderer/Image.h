#pragma once

#include "Texture.h"
#include "Shader.h"

class Image
{
public:
	void InitBuffers();
	Image(const std::string& path);
	~Image();
	void Draw();

public:
	const std::string m_title = "Image";
	const std::string m_vs_path = "res/shaders/image.vert";
	const std::string m_fs_path = "res/shaders/image.frag";

	float m_vertices[16] = {
	  // Positions   Textures
		 1.0f,  1.0f, 1.0f, 1.0f, // Top Right
		 1.0f, -1.0f, 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
		-1.0f,  1.0f, 0.0f, 1.0f  // Top Left 
	};

	const unsigned int m_indices[6] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	Shader m_shader;
	Texture m_texture;
	unsigned int m_vbo, m_ebo, m_vao;	
};
