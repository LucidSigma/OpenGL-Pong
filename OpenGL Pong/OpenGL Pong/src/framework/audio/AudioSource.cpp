#include "AudioSource.h"

AudioSource::AudioSource(const float volume, const float pitch, const glm::vec2 position, const glm::vec2 velocity, const bool isLooping)
	: m_volume(volume), m_pitch(pitch), m_position(position), m_velocity(velocity), m_isLooping(isLooping)
{
	alGenSources(1, &m_id);

	SetVolume(volume);
	SetPitch(pitch);
	SetPosition(position);
	SetVelocity(velocity);
	SetLooping(isLooping);
}

AudioSource::~AudioSource() noexcept
{
	alDeleteSources(1, &m_id);
}

void AudioSource::SetAttenuationVariables(const float rolloffFactor, const float referenceDistance, const float maxDistance) const
{
	alSourcef(m_id, AL_ROLLOFF_FACTOR, rolloffFactor);
	alSourcef(m_id, AL_REFERENCE_DISTANCE, referenceDistance);
	alSourcef(m_id, AL_MAX_DISTANCE, maxDistance);
}

void AudioSource::Play(const Sound& sound) const
{
	if (m_soundID != sound.GetID())
	{
		m_soundID = sound.GetID();
		alSourcei(m_id, AL_BUFFER, sound.GetID());
	}
	
	alSourcePlay(m_id);
}

void AudioSource::Pause() const
{
	alSourcePause(m_id);
}

void AudioSource::Resume() const
{
	alSourcePlay(m_id);
}

void AudioSource::Stop() const
{
	alSourceStop(m_id);
}

void AudioSource::Rewind() const
{
	alSourceRewind(m_id);
}

bool AudioSource::IsPlaying() const
{
	ALint sourceState = 0;
	alGetSourcei(m_id, AL_SOURCE_STATE, &sourceState);

	return sourceState == AL_PLAYING;
}

bool AudioSource::IsPaused() const
{
	ALint sourceState = 0;
	alGetSourcei(m_id, AL_SOURCE_STATE, &sourceState);

	return sourceState == AL_PAUSED;
}

bool AudioSource::IsStopped() const
{
	ALint sourceState = 0;
	alGetSourcei(m_id, AL_SOURCE_STATE, &sourceState);

	return sourceState == AL_STOPPED;
}

void AudioSource::SetVolume(const float volume) noexcept
{
	m_volume = volume;
	alSourcef(m_id, AL_GAIN, m_volume);
}

void AudioSource::SetPitch(const float pitch) noexcept
{
	m_pitch = pitch;
	alSourcef(m_id, AL_PITCH, m_pitch);
}

void AudioSource::SetPosition(const glm::vec2& position) noexcept
{
	m_position = position;
	alSource3f(m_id, AL_POSITION, m_position.x, m_position.y, 0.0f);
}

void AudioSource::SetVelocity(const glm::vec2& velocity) noexcept
{
	m_velocity = velocity;
	alSource3f(m_id, AL_VELOCITY, m_velocity.x, m_velocity.y, 0.0f);
}

void AudioSource::SetLooping(const bool isLooping) noexcept
{
	m_isLooping = isLooping;
	alSourcei(m_id, AL_LOOPING, m_isLooping ? AL_TRUE : AL_FALSE);
}