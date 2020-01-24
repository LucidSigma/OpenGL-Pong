#pragma once
#ifndef OUTLINE_H
#define OUTLINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>
#include <utility>

#include "shaders/ShaderProgram.h"

class Outline
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	const ShaderProgram& m_shader;

	glm::vec4 m_colour;

public:
	Outline(const ShaderProgram& shader, const glm::vec4& colour);
	~Outline() noexcept;

	void Render(const glm::vec2& spritePosition, const glm::vec2& spriteSize, const float spriteRotation, const float width) const;

private:
	void InitialiseVertexObjects() const;

	glm::mat4 CreateModelMatrix(const glm::vec2& position, const glm::vec2& size, const float rotation) const;
};

#endif