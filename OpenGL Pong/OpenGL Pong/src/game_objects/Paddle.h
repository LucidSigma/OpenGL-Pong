#pragma once
#ifndef PADDLE_H
#define PADDLE_H

#include "../framework/interfaces/IGameObject.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../framework/audio/AudioSource.h"
#include "../framework/graphics/Outline.h"
#include "../framework/graphics/Sprite.h"

class Paddle
	: public IGameObject
{
public:
	enum class Side
	{
		Left,
		Right
	};

private:
	static constexpr glm::vec2 s_Size{ 20.0f, 200.0f };
	static constexpr float s_Speed = 350.0f;

	glm::vec2 m_windowBounds;

	glm::vec2 m_position;
	glm::vec2 m_velocity{ 0.0f };

	Side m_side;

	Sprite m_sprite;
	Outline m_outline;

	AudioSource m_winSource;
	const Sound& m_winSound;

public:
	static constexpr const glm::vec2& GetSize() noexcept { return s_Size; }

	Paddle(const glm::vec2& windowBounds, const float x, const Side side, const ShaderProgram& shader, const ShaderProgram& outlineShader, const glm::vec4& outlineColour, const Texture& texture, const Sound& winSound);
	virtual ~Paddle() noexcept override = default;

	virtual void ProcessInput(const Uint8* keyStates) override;
	virtual void Update(const double deltaTime) override;
	virtual void Render() const override;

	void PlayWinSound() const;

	SDL_Rect GetRect() const;
};

#endif