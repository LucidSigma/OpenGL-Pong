#include "Game.h"

#include <glad/glad.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <SDL2/SDL.h>
#include <stb_image/stb_image_write.h>

#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "utility/ConfigReader.h"

Game::Game()
{
	Initialise();
}

Game::~Game() noexcept
{
	m_window = nullptr;

	SDL_Quit();
}

void Game::Play()
{
	SDL_Event gameEvent;

	while (m_isRunning)
	{
		PollEvents(gameEvent);
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Initialise()
{
	const Config config = ConfigReader().ReadConfigFile();

	InitialiseSDL(config);
	InitialiseWindow(config);
	InitialiseGLAD();
	InitialiseOpenGL();
	InitialiseAudio();

	stbi_flip_vertically_on_write(true);
}

void Game::InitialiseSDL(const Config& config) const
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to initialise SDL: " << SDL_GetError();

		throw std::runtime_error(errorMessageStream.str());
	}

	const std::initializer_list<std::pair<SDL_GLattr, int>> glAttributes{
		{ SDL_GL_DOUBLEBUFFER, SDL_TRUE },
		{ SDL_GL_CONTEXT_MAJOR_VERSION, config.glVersionMajor },
		{ SDL_GL_CONTEXT_MINOR_VERSION, config.glVersionMinor },
		{ SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE },
	#ifdef __APPLE__
		{ SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
	#endif
		{ SDL_GL_MULTISAMPLESAMPLES, config.multisampleCount },
		{ SDL_GL_STENCIL_SIZE, 8 }
	};

	for (const auto [attribute, value] : glAttributes)
	{
		SDL_GL_SetAttribute(attribute, value);
	}
}

void Game::InitialiseWindow(const Config& config)
{
	const SDL_Rect windowRect{
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(config.windowWidth),
		static_cast<int>(config.windowHeight)
	};

	m_window = std::make_unique<GLWindow>(config.title.data(), windowRect);
	m_window->SetIcon("assets/icon/icon.png");
	m_window->MakeCurrentContext();

	if (config.enableVsync)
	{
		if (SDL_GL_SetSwapInterval(true) != 0)
		{
			std::ostringstream errorMessageStream;
			errorMessageStream << "ERROR: Failed to setup Vsync: " << SDL_GetError();

			throw std::runtime_error(errorMessageStream.str());
		}
	}
}

void Game::InitialiseGLAD() const
{
	if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to initialise GLAD.";

		throw std::runtime_error(errorMessageStream.str());
	}
}

void Game::InitialiseOpenGL() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_MULTISAMPLE);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);

	glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Game::InitialiseAudio()
{
	m_audioContext.SetDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);

	m_audioContext.SetListenerPosition({ m_window->GetWidth() / 2.0f, m_window->GetHeight() / 2.0f, 0.0f });
	m_audioContext.SetListenerVelocity({ 0.0f, 0.0f, 0.0f });
	m_audioContext.SetListenerOrientation({ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
}

void Game::PollEvents(SDL_Event& gameEvent)
{
	while (SDL_PollEvent(&gameEvent))
	{
		switch (gameEvent.type)
		{
		case SDL_KEYDOWN:
			switch (gameEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_isRunning = false;

				break;

			case SDLK_F2:
				TakeScreenshot();

				break;

			default:
				break;
			}
			
			break;

		case SDL_QUIT:
			m_isRunning = false;

			break;

		default:
			break;
		}

		PollEventsForGameObjects(gameEvent);
	}
}

void Game::ProcessInput()
{
	const Uint8* keyStates = m_inputManager.GetKeyboardState();

	ProcessInputForGameObjects(keyStates);
}

void Game::Update()
{
	const double deltaTime = CalculateDeltaTime();

	UpdateGameObjects(deltaTime);
}

void Game::Render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	RenderGameObjects();

	m_window->SwapBuffers();
}

double Game::CalculateDeltaTime()
{
	constexpr double MillisecondsPerSecond = 1000.0;

	const double deltaTime = static_cast<double>(SDL_GetTicks() - m_ticksCount) / MillisecondsPerSecond;
	m_ticksCount = SDL_GetTicks();

	return deltaTime;
}

void Game::TakeScreenshot() const
{
	constexpr unsigned int PixelChannelCount = 3u;
	std::vector<std::byte> imageData(m_window->GetWidth() * m_window->GetHeight() * PixelChannelCount);

	glReadPixels(0, 0, m_window->GetWidth(), m_window->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

	const long long currentTime = std::chrono::system_clock::now().time_since_epoch().count();
	const std::string filename = "screenshots/screenshot_" + std::to_string(currentTime) + ".png";

	stbi_write_png(filename.c_str(), m_window->GetWidth(), m_window->GetHeight(), PixelChannelCount, imageData.data(), m_window->GetWidth() * PixelChannelCount);
}