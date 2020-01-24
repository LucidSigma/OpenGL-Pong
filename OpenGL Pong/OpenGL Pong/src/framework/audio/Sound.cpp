#include "Sound.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <stdexcept>

Sound::Sound(const std::string& filepath)
{
	alGenBuffers(1, &m_id);
	SetAudioBufferData(filepath);
}

Sound::~Sound() noexcept
{
	alDeleteBuffers(1, &m_id);
}

void Sound::SetAudioBufferData(const std::string& filepath) const
{
	SDL_AudioSpec audioSpec{ };
	unsigned int audioLength = 0;
	Uint8* audioData = nullptr;
	
	if (SDL_LoadWAV(filepath.c_str(), &audioSpec, &audioData, &audioLength) == nullptr)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to load audio file: " << filepath << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	const ALenum format = audioSpec.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	alBufferData(m_id, format, audioData, audioLength, audioSpec.freq);
	SDL_FreeWAV(audioData);
}