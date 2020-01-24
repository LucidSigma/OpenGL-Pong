#pragma once
#ifndef EFFECT_RENDERER_H
#define EFFECT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>
#include <memory>

#include "shaders/ShaderProgram.h"
#include "Texture.h"
#include "Vertex.h"

class EffectRenderer
{
private:
	static constexpr float s_KernelOffset = 1.0f / 350.0f;

	static constexpr std::array<glm::vec2, 9u> s_KernelOffsets{
		glm::vec2{ -s_KernelOffset, s_KernelOffset },
		glm::vec2{ 0.0f, s_KernelOffset },
		glm::vec2{ s_KernelOffset, s_KernelOffset },
		glm::vec2{ -s_KernelOffset, 0.0f },
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ s_KernelOffset, 0.0f },
		glm::vec2{ -s_KernelOffset, -s_KernelOffset },
		glm::vec2{ 0.0f, -s_KernelOffset },
		glm::vec2{ s_KernelOffset, -s_KernelOffset }
	};

	static constexpr std::array<float, 9u> s_BlurKernel{
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	GLuint m_multisampledFBO = 0;
	GLuint m_fbo = 0;
	GLuint m_colourRBO = 0;
	GLuint m_stencilRBO = 0;

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	glm::vec2 m_framebufferSize;

	const ShaderProgram& m_shader;
	std::unique_ptr<Texture> m_texture;

	bool m_active = false;

public:
	EffectRenderer(const glm::vec2& framebufferSize, const unsigned int sampleCount, const ShaderProgram& shader);
	~EffectRenderer() noexcept;

	void BeginEffectRender() const;
	void EndEffectRender() const;
	void Render() const;

	inline bool IsActive() const noexcept { return m_active; }
	inline void Activate() noexcept { m_active = true; }
	inline void Deactivate() noexcept { m_active = false; }

private:
	void InitialiseFramebuffers(const unsigned int sampleCount);
	void InitialiseVertexObjects() const;
};

#endif