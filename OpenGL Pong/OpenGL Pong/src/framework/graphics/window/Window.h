#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "../../interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include <memory>
#include <string>
#include <string_view>
#include <utility>

class Window
	: private INoncopyable
{
private:
	struct WindowDestroyer
	{
		void operator ()(SDL_Window* window) const noexcept;
	};

	std::unique_ptr<SDL_Window, WindowDestroyer> m_windowHandle = nullptr;

	unsigned int m_width = 0;
	unsigned int m_height = 0;

	unsigned int m_multisampleCount = 4;

public:
	Window() = default;
	Window(const std::string_view& title, const SDL_Rect& rect, const unsigned int multisampleCount = 4, const unsigned int flags = SDL_WINDOW_SHOWN);

	Window(Window&& other) noexcept;
	Window& operator =(Window&& other) noexcept;

	virtual ~Window() noexcept;

	void SetIcon(const std::string& iconFilepath);

	inline unsigned int GetWidth() const noexcept { return m_width; }
	inline unsigned int GetHeight() const noexcept { return m_height; }

	inline unsigned int GetMultisampleCount() const noexcept { return m_multisampleCount; }

	inline SDL_Window* GetPointer() const noexcept { return m_windowHandle.get(); }
	inline SDL_Surface* GetSurface() const noexcept { return SDL_GetWindowSurface(GetPointer()); }

protected:
	virtual void Destroy() noexcept;
};

#endif