#pragma once
#ifndef BALL_H
#define BALL_H

#include "../framework/interfaces/IGameObject.h"

#include <glm/glm.hpp>

#include "../framework/audio/AudioSource.h"
#include "../framework/graphics/Sprite.h"
#include "../framework/graphics/ParticleGenerator.h"
#include "Paddle.h"

class Ball
	: public IGameObject
{
private:
	static constexpr float s_Radius = 16.0f;
	static constexpr glm::vec2 s_InitialVelocity{ 500.0f, 500.0f };
	static constexpr unsigned int s_MaxParticles = 500u;

	glm::vec2 m_windowBounds;

	const glm::vec2 m_initialPosition;
	glm::vec2 m_position;
	glm::vec2 m_velocity = s_InitialVelocity;

	float m_rotation = 0.0f;

	Sprite m_sprite;
	ParticleGenerator m_particleGenerator;

	AudioSource m_collisionSource;
	const Sound& m_collisionSound;

public:
	Ball(const glm::vec2& windowBounds, const ShaderProgram& shader, const Texture& texture, const ShaderProgram& particleShader, const Texture& particleTexture, const Sound& collisionSound);
	virtual ~Ball() noexcept override = default;

	virtual void ProcessInput(const Uint8* keyStates) override;
	virtual void Update(const double deltaTime) override;
	virtual void Render() const override;

	void Reset();

	bool CollidesWithPaddle(const Paddle& paddle) const;
	void PlayCollisionSound() const;

	inline const glm::vec2& GetPosition() const noexcept { return m_position; }
	inline const glm::vec2& GetVelocity() const noexcept { return m_velocity; }
	inline void FlipXVelocity() noexcept { m_velocity.x *= -1.0f; }

	inline float GetRadius() const noexcept { return s_Radius; }

private:
	float GetRandomDirection() const;
};

#endif