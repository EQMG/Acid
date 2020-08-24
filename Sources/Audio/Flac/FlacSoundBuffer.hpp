#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT FlacSoundBuffer : public SoundBuffer::Registrar<FlacSoundBuffer> {
	static const bool Registered;
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};
}
