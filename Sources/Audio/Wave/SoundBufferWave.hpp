#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT SoundBufferWave : public SoundBuffer::Registrar<SoundBufferWave> {
	inline static const bool Registered = Register(".wav", ".wave");
public:
	static void Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename);
};
}
