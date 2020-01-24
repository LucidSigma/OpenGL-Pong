#include "Outline.h"

#include "Vertex.h"

Outline::Outline(const ShaderProgram& shader, const glm::vec4& colour)
	: m_shader(shader), m_colour(colour)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	InitialiseVertexObjects();
}

Outline::~Outline() noexcept
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Outline::Render(const glm::vec2& spritePosition, const glm::vec2& spriteSize, const float spriteRotation, const float width) const
{
	m_shader.Use();
	m_shader.SetUniform<glm::vec4>("u_Colour", m_colour);

	const glm::mat4 model = CreateModelMatrix(spritePosition - width, spriteSize + (width * 2.0f), spriteRotation);
	m_shader.SetUniform<glm::mat4>("u_Model", model);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void Outline::InitialiseVertexObjects() const
{
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices::OutlineVertices.size() * sizeof(std::pair<float, float>), vertices::OutlineVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices::QuadIndices.size() * sizeof(unsigned short), vertices::QuadIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(std::pair<float, float>), reinterpret_cast<const GLvoid*>(0));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glm::mat4 Outline::CreateModelMatrix(const glm::vec2& position, const glm::vec2& size, const float rotation) const
{
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3{ position, 0.0f });

	model = glm::translate(model, glm::vec3{ 0.5f * size.x, 0.5f * size.y, 0.0f });
	model = glm::rotate(model, rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
	model = glm::translate(model, glm::vec3{ -0.5f * size.x, -0.5f * size.y, 0.0f });

	model = glm::scale(model, glm::vec3{ size, 1.0f });

	return model;
}