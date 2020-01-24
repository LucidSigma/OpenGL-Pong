#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <string>

class Sound
{
private:
	ALuint m_id = 0;

public:
	Sound(const std::string& filepath);
	~Sound() noexcept;

	inline ALuint GetID() const noexcept { return m_id; }

private:
	void SetAudioBufferData(const std::string& filepath) const;
};

#endif