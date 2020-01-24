#include "Background.h"

Background::Background(const glm::vec2& size, const ShaderProgram& shader, const Texture& texture)
	: m_size(size), m_sprite(shader, texture)
{ }

void Background::ProcessInput(const Uint8 * keyStates)
{ }

void Background::Update(const double deltaTime)
{ }

void Background::Render() const
{
	m_sprite.Render(glm::vec2{ 0.0f }, m_size);
}