#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT SoundBufferWave : public SoundBuffer::Registrar<SoundBufferWave> {
public:
	static void Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename);

private:
	static bool registered;
};
}
