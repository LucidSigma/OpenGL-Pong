#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>

#include <tuple>

class InputManager
{
public:
	InputManager() = default;
	~InputManager() noexcept = default;

	const Uint8* GetKeyboardState() const;
	std::tuple<Uint32, float, float> GetMouseState() const;
};

#endif