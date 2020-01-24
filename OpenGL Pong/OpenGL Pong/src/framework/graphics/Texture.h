#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <unordered_map>
#include <string>

class Texture
{
private:
	inline static const std::unordered_map<int, std::pair<GLint, GLenum>> s_componentMap{
		{ 1, { GL_R8, GL_RED } },
		{ 2, { GL_RG8, GL_RG } },
		{ 3, { GL_RGB8, GL_RGB } },
		{ 4, { GL_RGBA8, GL_RGBA } }
	};

	GLuint m_id = 0;

	unsigned int m_width = 0;
	unsigned int m_height = 0;

public:
	explicit Texture(const std::string& filepath);
	explicit Texture(const glm::vec2& size);
	~Texture() noexcept;

	void Bind() const;

	inline GLuint GetID() const noexcept { return m_id; }

private:
	void Generate(const std::string& filepath);
	void SetupParameters(const GLint internalFormat, const GLenum format, const std::byte* data);
};

#endif