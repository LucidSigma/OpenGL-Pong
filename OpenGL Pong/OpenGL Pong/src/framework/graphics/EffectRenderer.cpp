#include "EffectRenderer.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <stdexcept>

EffectRenderer::EffectRenderer(const glm::vec2& framebufferSize, const unsigned int sampleCount, const ShaderProgram& shader)
	: m_framebufferSize(framebufferSize), m_shader(shader)
{
	glGenFramebuffers(1, &m_multisampledFBO);
	glGenFramebuffers(1, &m_fbo);
	glGenRenderbuffers(1, &m_colourRBO);
	glGenRenderbuffers(1, &m_stencilRBO);
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	
	InitialiseFramebuffers(sampleCount);
	InitialiseVertexObjects();
	
	m_shader.Use();
	m_shader.SetTextureUniform("u_Frame", 0);
	m_shader.SetUniformVector<glm::vec2>("u_KernelOffsets", s_KernelOffsets.data(), s_KernelOffsets.size());
	m_shader.SetUniformVector<float>("u_BlurKernel", s_BlurKernel.data(), s_BlurKernel.size());
}

EffectRenderer::~EffectRenderer() noexcept
{
	glDeleteFramebuffers(1, &m_multisampledFBO);
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteRenderbuffers(1, &m_colourRBO);

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void EffectRenderer::BeginEffectRender() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void EffectRenderer::EndEffectRender() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisampledFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glBlitFramebuffer(0, 0, static_cast<int>(m_framebufferSize.x), static_cast<int>(m_framebufferSize.y), 0, 0, static_cast<int>(m_framebufferSize.x), static_cast<int>(m_framebufferSize.y), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EffectRenderer::Render() const
{
	m_shader.Use();
	m_shader.SetUniform<float>("u_Time", static_cast<float>(SDL_GetTicks()));
	m_shader.SetUniform<bool>("u_Active", m_active);

	glActiveTexture(GL_TEXTURE0);
	m_texture->Bind();

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void EffectRenderer::InitialiseFramebuffers(const unsigned int sampleCount)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glBindRenderbuffer(GL_RENDERBUFFER, m_colourRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_RGB8, static_cast<int>(m_framebufferSize.x), static_cast<int>(m_framebufferSize.y));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colourRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, m_stencilRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_DEPTH24_STENCIL8, static_cast<int>(m_framebufferSize.x), static_cast<int>(m_framebufferSize.y));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilRBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to create multisampled effects framebuffer.";

		throw std::runtime_error(errorMessageStream.str());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	m_texture = std::make_unique<Texture>(m_framebufferSize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to create effects framebuffer.";

		throw std::runtime_error(errorMessageStream.str());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EffectRenderer::InitialiseVertexObjects() const
{
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices::FrameVertices.size() * sizeof(Vertex), vertices::FrameVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices::QuadIndices.size() * sizeof(unsigned short), vertices::QuadIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(Position);
	glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(TextureCoordinates);
	glVertexAttribPointer(TextureCoordinates, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, textureCoordinates)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}