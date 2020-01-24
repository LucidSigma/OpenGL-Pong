#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../framework/interfaces/IGameObject.h"

#include <glm/glm.hpp>

#include "../framework/graphics/Sprite.h"

class Background
	: public IGameObject
{
private:
	glm::vec2 m_size;
	Sprite m_sprite;

public:
	Background(const glm::vec2& size, const ShaderProgram& shader, const Texture& texture);
	virtual ~Background() noexcept override = default;

	virtual void ProcessInput(const Uint8 * keyStates) override;
	virtual void Update(const double deltaTime) override;
	virtual void Render() const override;
};

#endif