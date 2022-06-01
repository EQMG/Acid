#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_AUDIO_EXPORT OggSoundBuffer : public SoundBuffer::Registrar<OggSoundBuffer> {
	inline static const bool Registered = Register(".ogg");
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};
}
