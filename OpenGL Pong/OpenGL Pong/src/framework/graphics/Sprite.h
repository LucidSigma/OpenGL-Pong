#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>

#include "shaders/ShaderProgram.h"
#include "Texture.h"

class Sprite
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	const ShaderProgram& m_shader;
	const Texture& m_texture;

public:
	Sprite(const ShaderProgram& shader, const Texture& texture);
	~Sprite() noexcept;

	void Render(const glm::vec2& position, const glm::vec2& size, const float rotation = 0.0f, const glm::vec3& colour = glm::vec3{ 1.0f }) const;

private:
	void InitialiseVertexObjects() const;

	glm::mat4 CreateModelMatrix(const glm::vec2& position, const glm::vec2& size, const float rotation) const;
};

#endif