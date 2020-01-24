#include "Window.h"

#include <stb_image/stb_image.h>

#include <sstream>
#include <stdexcept>
#include <utility>

void Window::WindowDestroyer::operator ()(SDL_Window* window) const noexcept
{
	SDL_DestroyWindow(window);
}

Window::Window(const std::string_view& title, const SDL_Rect& rect, const unsigned int multisampleCount, const unsigned int flags)
	: m_windowHandle(SDL_CreateWindow(title.data(), rect.x, rect.y, rect.w, rect.h, flags)), m_width(rect.w), m_height(rect.h), m_multisampleCount(multisampleCount)
{
	if (m_windowHandle == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to create window: " << SDL_GetError();

		throw std::runtime_error(errorMessageStream.str());
	}
}

Window::Window(Window&& other) noexcept
	: m_windowHandle(nullptr), m_width(other.m_width), m_height(other.m_height), m_multisampleCount(other.m_multisampleCount)
{
	std::swap(this->m_windowHandle, other.m_windowHandle);

	other.m_width = 0;
	other.m_height = 0;

	other.m_multisampleCount = 0;
}

Window& Window::operator =(Window&& other) noexcept
{
	this->m_windowHandle = std::exchange(other.m_windowHandle, nullptr);

	this->m_width = std::exchange(other.m_width, 0);
	this->m_height = std::exchange(other.m_height, 0);

	this->m_multisampleCount = std::exchange(other.m_multisampleCount, 0);

	return *this;
}

Window::~Window() noexcept
{
	Destroy();
}

void Window::SetIcon(const std::string& iconFilepath)
{
	int width = 0;
	int height = 0;
	std::byte* iconData = reinterpret_cast<std::byte*>(stbi_load(iconFilepath.c_str(), &width, &height, nullptr, STBI_rgb_alpha));

	if (iconData == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to load icon file.";

		throw std::runtime_error(errorMessageStream.str());
	}

	Uint32 redMask = 0x00'00'00'FF;
	Uint32 blueMask = 0x00'00'FF'00;
	Uint32 greenMask = 0x00'FF'00'00;
	Uint32 alphaMask = 0xFF'00'00'00;

	if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		redMask = 0xFF'00'00'00;
		blueMask = 0x00'FF'00'00;
		greenMask = 0x00'00'FF'00;
		alphaMask = 0x00'00'00'FF;
	}

	SDL_Surface* iconSurface = SDL_CreateRGBSurfaceFrom(iconData, width, height, 32, width * 4, redMask, blueMask, greenMask, alphaMask);
	SDL_SetWindowIcon(GetPointer(), iconSurface);
	SDL_FreeSurface(iconSurface);

	stbi_image_free(iconData);
}

void Window::Destroy() noexcept
{
	if (m_windowHandle != nullptr)
	{
		m_windowHandle = nullptr;
	}
}