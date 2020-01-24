#pragma once
#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include "../shaders/ShaderProgram.h"
#include "Font.h"

class Text
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	const Font& m_font;
	const ShaderProgram& m_shader;

public:
	Text(const Font& font, const ShaderProgram& shader);
	~Text() noexcept;

	void Render(const std::string& text, glm::vec2 position, const float scale, const glm::vec3& colour = glm::vec3{ 1.0f });

private:
	void InitialiseVertexObjects() const;
};

#endif