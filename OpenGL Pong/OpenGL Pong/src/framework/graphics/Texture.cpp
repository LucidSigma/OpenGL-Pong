#include "Texture.h"

#include <stb_image/stb_image.h>

#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <utility>

Texture::Texture(const std::string& filepath)
{
	glGenTextures(1, &m_id);

	Generate(filepath);
}

Texture::Texture(const glm::vec2& size)
{
	glGenTextures(1, &m_id);

	m_width = static_cast<unsigned int>(size.x);
	m_height = static_cast<unsigned int>(size.y);

	SetupParameters(GL_RGB8, GL_RGB, nullptr);
}

Texture::~Texture() noexcept
{
	glDeleteTextures(1, &m_id);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Generate(const std::string& filepath)
{
	int width = 0;
	int height = 0;
	int componentCount = 0;

	std::byte* data = reinterpret_cast<std::byte*>(stbi_load(filepath.c_str(), &width, &height, &componentCount, STBI_default));

	if (data == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to load texture file: " << filepath << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	m_width = width;
	m_height = height;
	const auto [internalFormat, format] = s_componentMap.at(componentCount);

	SetupParameters(internalFormat, format, data);

	stbi_image_free(data);
}

void Texture::SetupParameters(const GLint internalFormat, const GLenum format, const std::byte* data)
{
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	const std::initializer_list<std::pair<GLenum, GLint>> textureParameters{
		{ GL_TEXTURE_WRAP_S, GL_REPEAT },
		{ GL_TEXTURE_WRAP_T, GL_REPEAT },
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR },
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR }
	};

	for (const auto [parameter, value] : textureParameters)
	{
		glTexParameteri(GL_TEXTURE_2D, parameter, value);
	}

	float maxAnisotropy = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);

	glBindTexture(GL_TEXTURE_2D, 0);
}