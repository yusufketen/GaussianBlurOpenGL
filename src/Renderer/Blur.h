#pragma once

#include "Texture.h"
#include "Shader.h"

class Blur
{
public:
	void InitBuffers();
	void InitFramebuffers();

	Blur(int width, int height);
	~Blur();

	void DrawHorizontal();
	void DrawVertical();

	void SetSigma(float sigma) { m_sigma = sigma; }

	unsigned int GetVAO() { return m_vao; }
	unsigned int GetFinalTextureID() { return m_texture_v.GetRendererID(); }
	unsigned int GetFinalFramebufferID() { return m_fbo_v; }

public:
	// Shader Names
	const std::string m_title_h = "Horizontal Blur";
	const std::string m_title_v = "Vertical Blur";

	// Vertex Shader
	const std::string m_vs_path = "res/shaders/blur.vert";

	// Fragment Shaders for vertical and horizontal passes
	const std::string m_fs_h_path = "res/shaders/blurH.frag";
	const std::string m_fs_v_path = "res/shaders/blurV.frag";

	float m_vertices[16] = {
		 // positions   // texCoords
		 1.0f,  1.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f,   0.0f, 1.0f  // top left 
	};

	const unsigned int m_indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int m_vbo, m_ebo, m_vao;

	Shader m_shader_h, m_shader_v;
	Texture m_texture_h, m_texture_v;
	unsigned int m_fbo_h, m_fbo_v;

	int m_width, m_height;
	float m_sigma;
};
