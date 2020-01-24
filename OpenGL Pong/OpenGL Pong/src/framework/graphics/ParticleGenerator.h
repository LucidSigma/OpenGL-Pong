#pragma once
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>
#include <cstddef>
#include <vector>

#include "shaders/ShaderProgram.h"
#include "Texture.h"

class ParticleGenerator
{
private:
	struct Particle
	{
		static constexpr float InitialScale = 10.0f;

		glm::vec2 position;
		glm::vec2 velocity;

		float scale = 10.0f;
		float life = 0.0f;
		float alpha = 1.0f;
	};

	const unsigned int m_maxAmount;
	std::vector<Particle> m_particles;

	const ShaderProgram& m_shader;
	const Texture& m_texture;

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

public:
	ParticleGenerator(const unsigned int maxAmount, const ShaderProgram& shader, const Texture& texture);
	~ParticleGenerator() noexcept;

	void Update(const double deltaTime, const glm::vec2& position, const glm::vec2& positionOffset, const glm::vec2& velocity, const unsigned int newParticleCount);
	void Render() const;

private:
	void InitialiseVertexObjects() const;

	size_t GetFirstDeadParticleIndex() const;
	void RespawnParticle(Particle& particle, const glm::vec2& position, const glm::vec2& positionOffset, const glm::vec2& velocity);
};

#endif