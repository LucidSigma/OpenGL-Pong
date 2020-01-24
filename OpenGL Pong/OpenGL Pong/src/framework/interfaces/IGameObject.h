#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL2/SDL.h>

class IGameObject
{
public:
	IGameObject() = default;
	virtual ~IGameObject() noexcept = default;

	virtual void ProcessInput(const Uint8* keyStates) = 0;
	virtual void Update(const double deltaTime) = 0;
	virtual void Render() const = 0;
};

#endif