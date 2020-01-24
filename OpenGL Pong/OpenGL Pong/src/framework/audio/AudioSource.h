#pragma once
#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include <glm/glm.hpp>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "Sound.h"

class AudioSource
{
private:
	ALuint m_id = 0;
	mutable ALuint m_soundID = 0;

	float m_volume = 1.0f;
	float m_pitch = 1.0f;

	glm::vec2 m_position{ 0.0f, 0.0f };
	glm::vec2 m_velocity{ 0.0f, 0.0f };

	bool m_isLooping = false;

public:
	explicit AudioSource(const float volume = 1.0f, const float pitch = 1.0f, const glm::vec2 position = glm::vec2{ 0.0f, 0.0f }, const glm::vec2 velocity = glm::vec2{ 0.0f, 0.0f }, const bool isLooping = false);
	~AudioSource() noexcept;

	void SetAttenuationVariables(const float rolloffFactor, const float referenceDistance, const float maxDistance) const;

	void Play(const Sound& sound) const;
	void Pause() const;
	void Resume() const;
	void Stop() const;
	void Rewind() const;
	
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;

	inline ALuint GetID() const noexcept { return m_id; }

	inline float GetVolume() const noexcept { return m_volume; }
	void SetVolume(const float volume) noexcept;

	inline float GetPitch() const noexcept { return m_pitch; }
	void SetPitch(const float pitch) noexcept;

	inline const glm::vec2& GetPosition() const noexcept { return m_position; }
	void SetPosition(const glm::vec2& position) noexcept;

	inline const glm::vec2& GetVelocity() const noexcept { return m_velocity; }
	void SetVelocity(const glm::vec2& velocity) noexcept;

	inline bool IsLooping() const noexcept { return m_isLooping; }
	void SetLooping(const bool isLooping) noexcept;
};

#endif