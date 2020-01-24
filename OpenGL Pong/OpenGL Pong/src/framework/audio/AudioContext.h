#pragma once
#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#include <glm/glm.hpp>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class AudioContext
{
private: 
	ALCdevice* m_audioDevice = nullptr;
	ALCcontext* m_audioContext = nullptr;

public:
	AudioContext();
	~AudioContext() noexcept;

	void SetDistanceModel(const ALenum distanceModel) const;

	void SetListenerPosition(const glm::vec3& position) const;
	void SetListenerVelocity(const glm::vec3& velocity) const;
	void SetListenerOrientation(const glm::vec3& lookingDirection, const glm::vec3& up) const;
};

#endif