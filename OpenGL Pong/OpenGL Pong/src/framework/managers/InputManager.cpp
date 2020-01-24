#include "InputManager.h"

const Uint8* InputManager::GetKeyboardState() const
{
	return SDL_GetKeyboardState(nullptr);
}

std::tuple<Uint32, float, float> InputManager::GetMouseState() const
{
	int x = 0;
	int y = 0;
	const Uint32 mouseState = SDL_GetMouseState(&x, &y);

	return { mouseState, static_cast<float>(x), static_cast<float>(y) };
}