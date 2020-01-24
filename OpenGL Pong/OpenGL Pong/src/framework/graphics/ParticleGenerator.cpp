#include "ParticleGenerator.h"

#include <algorithm>

#include "../utility/Random.h"
#include "Vertex.h"

ParticleGenerator::ParticleGenerator(const unsigned int maxAmount, const ShaderProgram& shader, const Texture& texture)
	: m_maxAmount(maxAmount), m_shader(shader), m_texture(texture)
{
	m_particles.resize(maxAmount);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	InitialiseVertexObjects();
}

ParticleGenerator::~ParticleGenerator() noexcept
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void ParticleGenerator::Update(const double deltaTime, const glm::vec2& position, const glm::vec2& positionOffset, const glm::vec2& velocity, const unsigned int newParticleCount)
{
	for (unsigned int i = 0; i < newParticleCount; ++i)
	{
		const size_t firstDeadParticleIndex = GetFirstDeadParticleIndex();
		RespawnParticle(m_particles[firstDeadParticleIndex], position, positionOffset, velocity);
	}

	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		Particle& particle = m_particles[i];

		if (particle.life > 0.0f)
		{
			particle.life -= static_cast<float>(deltaTime);
			particle.position -= particle.velocity * static_cast<float>(deltaTime);
			particle.scale -= 5.0f * static_cast<float>(deltaTime);
			particle.alpha -= 1.5f * static_cast<float>(deltaTime);
		}
	}
}

void ParticleGenerator::Render() const
{
	m_shader.Use();

	for (const Particle& particle : m_particles)
	{
		if (particle.life > 0.0f)
		{
			m_shader.SetUniform<glm::vec2>("u_PositionOffset", particle.position);
			m_shader.SetUniform<float>("u_Scale", particle.scale);
			m_shader.SetUniform<float>("u_Alpha", particle.alpha);
			m_texture.Bind();

			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
			glBindVertexArray(0);
		}
	}
}

void ParticleGenerator::InitialiseVertexObjects() const
{
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices::QuadVertices.size() * sizeof(Vertex), vertices::QuadVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices::QuadIndices.size() * sizeof(unsigned short), vertices::QuadIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(Position);
	glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(TextureCoordinates);
	glVertexAttribPointer(TextureCoordinates, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, textureCoordinates)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t ParticleGenerator::GetFirstDeadParticleIndex() const
{
	static size_t firstDeadParticleIndex = 0;

	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		if (m_particles[i].life <= 0.0f)
		{
			firstDeadParticleIndex = i;

			return i;
		}
	}

	for (size_t i = 0; i < firstDeadParticleIndex; ++i)
	{
		if (m_particles[i].life <= 0.0f)
		{
			firstDeadParticleIndex = i;

			return i;
		}
	}

	firstDeadParticleIndex = 0;

	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, const glm::vec2& position, const glm::vec2& positionOffset, const glm::vec2& velocity)
{
	constexpr float RandomOffsetLimit = 7.5f;
	particle.position = position + positionOffset + glm::vec2{ Random::GenerateFloat(-RandomOffsetLimit, RandomOffsetLimit), Random::GenerateFloat(-RandomOffsetLimit, RandomOffsetLimit) };

	constexpr float RandomVelocityLimit = 40.0f;
	particle.velocity = velocity * 0.1f + glm::vec2{ Random::GenerateFloat(-RandomVelocityLimit, RandomVelocityLimit), Random::GenerateFloat(-RandomVelocityLimit, RandomVelocityLimit) };

	particle.scale = Particle::InitialScale;
	particle.life = 1.0f;
	particle.alpha = 1.0f;
}