#include "Ball.h"

#include <cmath>

#include "../framework/utility/Random.h"

Ball::Ball(const glm::vec2& windowBounds, const ShaderProgram& shader, const Texture& texture, const ShaderProgram& particleShader, const Texture& particleTexture, const Sound& collisionSound)
	: m_windowBounds(windowBounds), m_initialPosition({ (m_windowBounds.x / 2.0f) - s_Radius, (m_windowBounds.y / 2.0f) - s_Radius }), m_position(m_initialPosition), m_sprite(shader, texture), m_particleGenerator(s_MaxParticles, particleShader, particleTexture), m_collisionSource(1.0f, 1.0f, m_position), m_collisionSound(collisionSound)
{
	m_velocity.x *= GetRandomDirection();
	m_velocity.y *= GetRandomDirection();

	m_collisionSource.SetAttenuationVariables(0.0f, 1.0f, m_windowBounds.x + m_windowBounds.y);
}

void Ball::ProcessInput(const Uint8* keyStates)
{ }

void Ball::Update(const double deltaTime)
{
	m_position += m_velocity * static_cast<float>(deltaTime);

	if (m_position.y < 0.0f)
	{
		m_position.y = 0.0f;
		m_velocity.y *= -1.0f;

		PlayCollisionSound();
	}
	else if (m_position.y > m_windowBounds.y - (s_Radius * 2.0f))
	{
		m_position.y = m_windowBounds.y - (s_Radius * 2.0f);
		m_velocity.y *= -1.0f;

		PlayCollisionSound();
	}

	m_collisionSource.SetPosition(m_position);
	m_rotation = static_cast<float>(SDL_GetTicks() * deltaTime);

	m_particleGenerator.Update(deltaTime, m_position, glm::vec2{ s_Radius / 2.0f }, m_velocity, 2);
}

void Ball::Render() const
{
	m_particleGenerator.Render();
	m_sprite.Render(m_position, glm::vec2{ s_Radius * 2.0f, s_Radius * 2.0f }, m_rotation);
}

void Ball::Reset()
{
	m_position = m_initialPosition;
	m_velocity = s_InitialVelocity;

	m_velocity.x *= GetRandomDirection();
	m_velocity.y *= GetRandomDirection();

	m_collisionSource.SetPosition(m_position);
}

bool Ball::CollidesWithPaddle(const Paddle& paddle) const
{
	const SDL_Rect rect = paddle.GetRect();
	const glm::vec2 centre{ m_position.x + s_Radius, m_position.y + s_Radius };
	const glm::vec2 circleDistance{ std::fabsf(centre.x - (rect.x + rect.w / 2.0f)), std::fabsf(centre.y - (rect.y + rect.h / 2.0f)) };

	if (circleDistance.x > rect.w / 2.0f + s_Radius ||
		circleDistance.y > rect.h / 2.0f + s_Radius)
	{
		return false;
	}

	if (circleDistance.x <= rect.w / 2.0f ||
		circleDistance.y <= rect.h / 2.0f)
	{
		return true;
	}

	const float cornerDistanceSquared = std::powf(circleDistance.x - rect.w / 2.0f, 2) + std::powf(circleDistance.y - rect.h / 2.0f, 2);

	return cornerDistanceSquared <= s_Radius * s_Radius;
}

void Ball::PlayCollisionSound() const
{
	m_collisionSource.Play(m_collisionSound);
}

float Ball::GetRandomDirection() const
{
	const unsigned int randomNumber = Random::GenerateInt(0, 1);

	return randomNumber > 0.5f ? 1.0f : -1.0f;
}