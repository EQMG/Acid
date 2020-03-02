#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT SoundBufferMp3 : public SoundBuffer::Registrar<SoundBufferMp3> {
	inline static const bool Registered = Register(".mp3");
public:
	static void Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename);
};
}
