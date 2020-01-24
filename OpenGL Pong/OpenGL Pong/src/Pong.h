#pragma once
#ifndef PONG_H
#define PONG_H

#include "framework/Game.h"

#include <array>
#include <memory>

#include "framework/audio/AudioSource.h"
#include "framework/graphics/EffectRenderer.h"
#include "framework/graphics/text/Text.h"
#include "framework/managers/FontManager.h"
#include "framework/managers/ShaderManager.h"
#include "framework/managers/SoundManager.h"
#include "framework/managers/TextureManager.h"
#include "game_objects/Background.h"
#include "game_objects/Ball.h"
#include "game_objects/Paddle.h"

class Pong
	: public Game
{
private:
	enum class State
	{
		Start,
		Running,
		Paused
	};

	ShaderManager m_shaders;
	TextureManager m_textures;
	FontManager m_fonts;
	SoundManager m_sounds;

	State m_state = State::Start;

	std::unique_ptr<EffectRenderer> m_shakeEffect;
	float m_shakeTime = 0.0f;

	glm::mat4 m_worldProjectionMatrix;
	std::unique_ptr<Background> m_background;
	std::unique_ptr<AudioSource> m_musicSource;

	std::unique_ptr<Ball> m_ball;

	std::array<std::unique_ptr<Paddle>, 2u> m_players;
	std::array<unsigned int, 2u> m_playerScores{ 0, 0 };

	std::array<std::unique_ptr<Text>, 2u> m_scoreTexts;

public:
	Pong();
	virtual ~Pong() noexcept override = default;

protected:
	virtual void PollEventsForGameObjects(const SDL_Event& gameEvent) override;
	virtual void ProcessInputForGameObjects(const Uint8* keyStates) override;
	virtual void UpdateGameObjects(const double deltaTime) override;
	virtual void RenderGameObjects() const override;

private:
	void InitialiseAssets();
	void InitialiseGameObjects();
};

#endif