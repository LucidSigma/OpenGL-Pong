#pragma once
#ifndef GAME_H
#define GAME_H

#include "interfaces/INoncopyable.h"
#include "interfaces/INonmovable.h"

#include "audio/AudioContext.h"
#include "graphics/window/GLWindow.h"
#include "managers/InputManager.h"
#include "utility/Config.h"

class Game
	: private INoncopyable, private INonmovable
{
protected:
	std::unique_ptr<GLWindow> m_window = nullptr;

private:
	bool m_isRunning = true;
	Uint32 m_ticksCount = 0;
	
	AudioContext m_audioContext;
	InputManager m_inputManager;

public:
	Game();
	virtual ~Game() noexcept;

	void Play();

protected:
	virtual void PollEventsForGameObjects(const SDL_Event& gameEvent) = 0;
	virtual void ProcessInputForGameObjects(const Uint8* keyStates) = 0;
	virtual void UpdateGameObjects(const double deltaTime) = 0;
	virtual void RenderGameObjects() const = 0;

private:
	void Initialise();

	void InitialiseSDL(const Config& config) const;
	void InitialiseWindow(const Config& config);
	void InitialiseGLAD() const;
	void InitialiseOpenGL() const;
	void InitialiseAudio();

	void PollEvents(SDL_Event& gameEvent);
	void ProcessInput();
	void Update();
	void Render() const;

	double CalculateDeltaTime();

	void TakeScreenshot() const;
};

#endif