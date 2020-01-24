#include "ShaderProgram.h"

#include <iterator>
#include <sstream>
#include <vector>

#include "../../utility/Logger.h"

ShaderProgram::ShaderProgram(const std::initializer_list<Shader>& shaders)
	: m_id(glCreateProgram())
{
	for (const auto& shader : shaders)
	{
		glAttachShader(m_id, shader.GetID());
	}

	glLinkProgram(m_id);

	if (!CheckLinkingStatus())
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to link program " << m_id << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	glValidateProgram(m_id);

	if (!CheckValidationStatus())
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to validate program " << m_id << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	for (const auto& shader : shaders)
	{
		glDetachShader(m_id, shader.GetID());
	}
}

ShaderProgram::~ShaderProgram() noexcept
{
	glDeleteProgram(m_id);
}

void ShaderProgram::Use() const
{
	glUseProgram(m_id);
}

void ShaderProgram::SetTextureUniform(const std::string& name, const int index) const
{
	const GLint location = GetUniformLocation(name);
	glUniform1i(location, index);
}

bool ShaderProgram::CheckLinkingStatus() const
{
	if (!CheckStatus(GL_LINK_STATUS))
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog(infoLogLength);
		glGetProgramInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

		std::ostringstream warningMessageStream;
		warningMessageStream << "SHADER PROGRAM LINKAGE LOG:\n" << infoLog.data();

		Logger::Log(warningMessageStream.str());

		return false;
	}

	return true;
}

bool ShaderProgram::CheckValidationStatus() const
{
	if (!CheckStatus(GL_VALIDATE_STATUS))
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog(infoLogLength);
		glGetProgramInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

		std::ostringstream warningMessageStream;
		warningMessageStream << "SHADER PROGRAM VALIDATION LOG:\n" << infoLog.data();

		Logger::Log(warningMessageStream.str());

		return false;
	}

	return true;
}

bool ShaderProgram::CheckStatus(const GLenum status) const
{
	GLint isSuccessful = 0;
	glGetProgramiv(m_id, status, &isSuccessful);

	return isSuccessful == GL_TRUE;
}

int ShaderProgram::GetUniformLocation(const std::string& uniformName) const
{
	if (m_uniformCache.find(uniformName) != std::cend(m_uniformCache))
	{
		return m_uniformCache[uniformName];
	}

	const int location = glGetUniformLocation(m_id, uniformName.c_str());

	if (location == GL_INVALID_INDEX)
	{
		std::ostringstream warningMessageStream;
		warningMessageStream << "WARNING: Uniform " << uniformName << " not found in shader program " << m_id << ".";

		Logger::Log(warningMessageStream.str());
	}

	m_uniformCache[uniformName] = location;

	return location;
}