#include "Pong.h"

#include <glm/glm.hpp>

#include <filesystem>

Pong::Pong()
{
	InitialiseAssets();
	InitialiseGameObjects();
}

void Pong::PollEventsForGameObjects(const SDL_Event& gameEvent)
{
	switch (gameEvent.type)
	{
	case SDL_WINDOWEVENT:
		switch (gameEvent.window.event)
		{
		case SDL_WINDOWEVENT_FOCUS_LOST:
			if (m_state == State::Running)
			{
				m_state = State::Paused;
			}

			if (m_musicSource->IsPlaying())
			{
				m_musicSource->Pause();
			}

			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			if (m_state == State::Paused)
			{
				m_state = State::Running;
			}

			if (m_musicSource->IsPaused())
			{
				m_musicSource->Resume();
			}

			break;

		default:
			break;
		}

		break;

	default:
		break;
	}
}

void Pong::ProcessInputForGameObjects(const Uint8 * keyStates)
{
	if (m_state == State::Start)
	{
		if (keyStates[SDL_SCANCODE_RETURN] || keyStates[SDL_SCANCODE_KP_ENTER])
		{
			m_state = State::Running;
		}
	}
	else if (m_state == State::Running)
	{
		for (const auto& player : m_players)
		{
			player->ProcessInput(keyStates);
		}
	}
}

void Pong::UpdateGameObjects(const double deltaTime)
{
	if (m_state == State::Running)
	{
		for (const auto& player : m_players)
		{
			player->Update(deltaTime);
		}

		m_ball->Update(deltaTime);

		if ((m_ball->CollidesWithPaddle(*m_players[0]) && m_ball->GetVelocity().x < 0.0f) ||
			(m_ball->CollidesWithPaddle(*m_players[1]) && m_ball->GetVelocity().x > 0.0f))
		{
			constexpr float InitialShakeTime = 0.25f;
			m_shakeTime = InitialShakeTime;
			m_shakeEffect->Activate();

			m_ball->PlayCollisionSound();
			m_ball->FlipXVelocity();
		}

		if (m_ball->GetPosition().x < 0.0f)
		{
			m_ball->Reset();
			++m_playerScores[1];

			m_players[1]->PlayWinSound();
		}
		else if (m_ball->GetPosition().x > m_window->GetWidth() - (m_ball->GetRadius() * 2.0f))
		{
			m_ball->Reset();
			++m_playerScores[0];

			m_players[0]->PlayWinSound();
		}

		if (m_shakeTime > 0.0f)
		{
			m_shakeTime -= static_cast<float>(deltaTime);
		}

		if (m_shakeTime <= 0.0f)
		{
			m_shakeEffect->Deactivate();
		}
	}
}

void Pong::RenderGameObjects() const
{
	if (m_shakeEffect->IsActive())
	{
		m_shakeEffect->BeginEffectRender();
	}

	m_background->Render();

	for (const auto& player : m_players)
	{
		player->Render();
	}

	m_ball->Render();

	if (m_shakeEffect->IsActive())
	{
		m_shakeEffect->EndEffectRender();
		m_shakeEffect->Render();
	}

	constexpr glm::vec2 TextOffset{ 20.0f, 20.0f };
	constexpr float CharacterSize = 32.0f;

	constexpr auto DigitCount = [](unsigned int number) -> unsigned int
	{
		if (number == 0)
		{
			return 1;
		}

		unsigned int count = 0;

		while (number != 0)
		{
			number /= 10;
			++count;
		}

		return count;
	};

	m_scoreTexts[0]->Render(std::to_string(m_playerScores[0]), TextOffset, 1.0f, glm::vec3{ 1.0f, 0.0f, 0.0f });
	m_scoreTexts[1]->Render(std::to_string(m_playerScores[1]), glm::vec2{ m_window->GetWidth() - (DigitCount(m_playerScores[1]) * CharacterSize), TextOffset.y * 2.0f } -TextOffset, 1.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
}

void Pong::InitialiseAssets()
{
	m_worldProjectionMatrix = glm::ortho(0.0f, static_cast<float>(m_window->GetWidth()), static_cast<float>(m_window->GetHeight()), 0.0f, -1.0f, 1.0f);

	m_shaders.AddShader("sprite", { Shader(Shader::Type::Vertex, "assets/shaders/sprite.vert"), Shader(Shader::Type::Fragment, "assets/shaders/sprite.frag") });
	m_shaders["sprite"].Use();
	m_shaders["sprite"].SetUniform<glm::mat4>("u_Projection", m_worldProjectionMatrix);
	m_shaders["sprite"].SetTextureUniform("u_Sprite", 0);

	m_shaders.AddShader("outline", { Shader(Shader::Type::Vertex, "assets/shaders/outline.vert"), Shader(Shader::Type::Fragment, "assets/shaders/outline.frag") });
	m_shaders["outline"].Use();
	m_shaders["outline"].SetUniform<glm::mat4>("u_Projection", m_worldProjectionMatrix);

	m_shaders.AddShader("particle", { Shader(Shader::Type::Vertex, "assets/shaders/particle.vert"), Shader(Shader::Type::Fragment, "assets/shaders/particle.frag") });
	m_shaders["particle"].Use();
	m_shaders["particle"].SetUniform<glm::mat4>("u_Projection", m_worldProjectionMatrix);
	m_shaders["particle"].SetTextureUniform("u_Particle", 0);

	m_shaders.AddShader("shake", { Shader(Shader::Type::Vertex, "assets/shaders/shake.vert"), Shader(Shader::Type::Fragment, "assets/shaders/shake.frag") });
	m_shaders["shake"].Use();
	m_shaders["shake"].SetTextureUniform("u_Frame", 0);

	m_shaders.AddShader("text", { Shader(Shader::Type::Vertex, "assets/shaders/text.vert"), Shader(Shader::Type::Fragment, "assets/shaders/text.frag") });
	m_shaders["text"].Use();
	m_shaders["text"].SetUniform<glm::mat4>("u_Projection", m_worldProjectionMatrix);
	m_shaders["text"].SetTextureUniform("u_Text", 0);

	m_fonts.AddFont("score", "assets/fonts/nasalization-rg.ttf", 48);

	for (const auto& textureFile : std::filesystem::directory_iterator("assets/textures"))
	{
		m_textures.AddTexture(textureFile.path().stem().string(), textureFile.path().string());
	}

	for (const auto& soundEffectFile : std::filesystem::directory_iterator("assets/audio/sounds"))
	{
		m_sounds.AddSound(soundEffectFile.path().stem().string(), soundEffectFile.path().string());
	}

	for (const auto& musicFile : std::filesystem::directory_iterator("assets/audio/music"))
	{
		m_sounds.AddSound(musicFile.path().stem().string(), musicFile.path().string());
	}
}

void Pong::InitialiseGameObjects()
{
	m_shakeEffect = std::make_unique<EffectRenderer>(glm::vec2{ m_window->GetWidth(), m_window->GetHeight() }, m_window->GetMultisampleCount(), m_shaders["shake"]);

	const glm::vec2 windowBounds{ m_window->GetWidth(), m_window->GetHeight() };
	m_background = std::make_unique<Background>(windowBounds, m_shaders["sprite"], m_textures["background"]);
	m_ball = std::make_unique<Ball>(windowBounds, m_shaders["sprite"], m_textures["ball"], m_shaders["particle"], m_textures["particle"], m_sounds["hit"]);

	const float paddleOffset = 40.0f;

	m_players[0] = std::make_unique<Paddle>(windowBounds, paddleOffset, Paddle::Side::Left, m_shaders["sprite"], m_shaders["outline"], glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, m_textures["paddle"], m_sounds["left_win"]);
	m_players[1] = std::make_unique<Paddle>(windowBounds, m_window->GetWidth() - Paddle::GetSize().x - paddleOffset, Paddle::Side::Right, m_shaders["sprite"], m_shaders["outline"], glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, m_textures["paddle"], m_sounds["right_win"]);

	for (auto& text : m_scoreTexts)
	{
		text = std::make_unique<Text>(m_fonts["score"], m_shaders["text"]);
	}

	m_musicSource = std::make_unique<AudioSource>(1.0f, 1.0f, glm::vec2{ m_window->GetWidth() / 2.0f, m_window->GetHeight() / 2.0f }, glm::vec2{ 0.0f, 0.0f }, true);
	m_musicSource->SetAttenuationVariables(0.0f, 1.0f, static_cast<float>(m_window->GetWidth() + m_window->GetHeight()));
	m_musicSource->Play(m_sounds["edm-detection-mode"]);
}