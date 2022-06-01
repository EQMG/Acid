#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_AUDIO_EXPORT Mp3SoundBuffer : public SoundBuffer::Registrar<Mp3SoundBuffer> {
	inline static const bool Registered = Register(".mp3");
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};
}
