#include "Paddle.h"

Paddle::Paddle(const glm::vec2& windowBounds, const float x, const Side side, const ShaderProgram& shader, const ShaderProgram& outlineShader, const glm::vec4& outlineColour, const Texture& texture, const Sound& winSound)
	: m_windowBounds(windowBounds), m_position(x, m_windowBounds.y / 2.0f - s_Size.y / 2.0f), m_side(side), m_sprite(shader, texture), m_outline(outlineShader, outlineColour), m_winSource(1.0f, 1.0f, m_position), m_winSound(winSound)
{
	m_winSource.SetAttenuationVariables(0.0f, 1.0f, m_windowBounds.x + m_windowBounds.y);
}

void Paddle::ProcessInput(const Uint8* keyStates)
{
	const int upKeyScancode = m_side == Side::Left ? SDL_SCANCODE_W : SDL_SCANCODE_UP;
	const int downKeyScancode = m_side == Side::Left ? SDL_SCANCODE_S : SDL_SCANCODE_DOWN;

	m_velocity.y = 0.0f;

	if (keyStates[upKeyScancode])
	{
		m_velocity.y -= s_Speed;
	}
	
	if (keyStates[downKeyScancode])
	{
		m_velocity.y += s_Speed;
	}
}

void Paddle::Update(const double deltaTime)
{
	m_position += m_velocity * static_cast<float>(deltaTime);

	if (m_position.y < 0.0f)
	{
		m_position.y = 0.0f;
	}
	else if (m_position.y > m_windowBounds.y - s_Size.y)
	{
		m_position.y = m_windowBounds.y - s_Size.y;
	}

	m_winSource.SetPosition(m_position);
}

void Paddle::Render() const
{
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	m_sprite.Render(m_position, s_Size);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	m_outline.Render(m_position, s_Size, 0.0f, 5.0f);

	glDisable(GL_STENCIL_TEST);
}

void Paddle::PlayWinSound() const
{
	m_winSource.Play(m_winSound);
}

SDL_Rect Paddle::GetRect() const
{
	return SDL_Rect{
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		static_cast<int>(s_Size.x),
		static_cast<int>(s_Size.y)
	};
}