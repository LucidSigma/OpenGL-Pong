#include "AudioContext.h"

#include <array>
#include <sstream>
#include <stdexcept>

AudioContext::AudioContext()
{
	m_audioDevice = alcOpenDevice(nullptr);

	if (m_audioDevice == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to initialise OpenAL device.";

		throw std::runtime_error(errorMessageStream.str());
	}

	m_audioContext = alcCreateContext(m_audioDevice, nullptr);

	if (m_audioContext == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to create OpenAL context.";

		throw std::runtime_error(errorMessageStream.str());
	}

	if (alcMakeContextCurrent(m_audioContext) == AL_FALSE)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed make a current OpenAL context.";

		throw std::runtime_error(errorMessageStream.str());
	}
}

AudioContext::~AudioContext() noexcept
{
	m_audioDevice = alcGetContextsDevice(m_audioContext);
	alcMakeContextCurrent(nullptr);

	alcDestroyContext(m_audioContext);
	alcCloseDevice(m_audioDevice);
}

void AudioContext::SetDistanceModel(const ALenum distanceModel) const
{
	alDistanceModel(distanceModel);
}

void AudioContext::SetListenerPosition(const glm::vec3& position) const
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioContext::SetListenerVelocity(const glm::vec3& velocity) const
{
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioContext::SetListenerOrientation(const glm::vec3& lookingDirection, const glm::vec3& up) const
{
	const std::array<float, 6u> listenerOrientation{
		lookingDirection.x, lookingDirection.y, lookingDirection.z,
		up.x, up.y, up.z
	};

	alListenerfv(AL_ORIENTATION, listenerOrientation.data());
}