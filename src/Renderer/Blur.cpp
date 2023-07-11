#include "Blur.h"

Blur::Blur(int width, int height) :
	m_shader_h(m_title_h, m_vs_path, m_fs_h_path),
	m_shader_v(m_title_v, m_vs_path, m_fs_v_path),
	m_texture_h(width, height),
	m_texture_v(width, height)
{
	m_width = width;
	m_height = height;

	InitBuffers();
	InitFramebuffers();
}

Blur::~Blur()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);

	m_shader_h.Destroy();
	m_shader_v.Destroy();

	glDeleteFramebuffers(1, &m_fbo_h);
	glDeleteFramebuffers(1, &m_fbo_v);
}

void Blur::InitBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Blur::InitFramebuffers()
{
	glGenFramebuffers(1, &m_fbo_h);
	glGenFramebuffers(1, &m_fbo_v);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_h);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_h.GetRendererID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_v);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_v.GetRendererID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Blur::DrawHorizontal()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader_h.Bind();
	m_shader_h.Uniform1f("width", static_cast<float>(m_width));
	m_shader_h.Uniform1f("sigma", m_sigma);
}

void Blur::DrawVertical()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_v);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader_v.Bind();
	m_shader_v.Uniform1f("height", static_cast<float>(m_height));
	m_shader_v.Uniform1f("sigma", m_sigma);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_h.GetRendererID());
}
