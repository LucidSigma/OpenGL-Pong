#include "Shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../../utility/Logger.h"

Shader::Shader(const Type type, const std::string& filepath, const bool isSpirV)
	: m_id(glCreateShader(static_cast<GLenum>(type)))
{
	const std::string& shaderCode = ReadShaderFile(filepath, isSpirV);
	const char* shaderCodeData = shaderCode.c_str();

	if (isSpirV)
	{
		CompileSpirVShader(shaderCode);
	}
	else
	{
		CompileShader(shaderCodeData);
	}

	if (!CheckCompilationErrors())
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to compile shader: " << filepath << ".";

		throw std::runtime_error(errorMessageStream.str());
	}
}

Shader::~Shader() noexcept
{
	glDeleteShader(m_id);
}

std::string Shader::ReadShaderFile(const std::string& filepath, const bool isSpirV)
{
	const int fileFlags = isSpirV ? std::ios_base::in | std::ios_base::binary : std::ios_base::in;
	std::ifstream shaderFile(filepath, fileFlags);

	if (!shaderFile.is_open())
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to open shader file: " << filepath << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	std::ostringstream fileReader;
	fileReader << shaderFile.rdbuf();
	shaderFile.close();

	return fileReader.str();
}

void Shader::CompileShader(const char* shaderCodeData)
{
	glShaderSource(m_id, 1, &shaderCodeData, nullptr);
	glCompileShader(m_id);
}

void Shader::CompileSpirVShader(const std::string& shaderCode)
{
	glShaderBinary(1, &m_id, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderCode.data(), static_cast<GLsizei>(shaderCode.length()));
	glSpecializeShader(m_id, "main", 0, nullptr, nullptr);
}

bool Shader::CheckCompilationErrors() const
{
	GLint isSuccessful = 0;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &isSuccessful);

	if (isSuccessful == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog(infoLogLength);
		glGetShaderInfoLog(m_id, infoLogLength, nullptr, infoLog.data());

		std::ostringstream warningMessageStream;
		warningMessageStream << "SHADER COMPILATION LOG:\n" << infoLog.data();

		Logger::Log(warningMessageStream.str());

		return false;
	}

	return true;
}