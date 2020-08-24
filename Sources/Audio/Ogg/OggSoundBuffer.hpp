#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT OggSoundBuffer : public SoundBuffer::Registrar<OggSoundBuffer> {
	static const bool Registered;
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};
}
