#pragma once

#include "Audio/SoundBuffer.hpp"

namespace acid {
class ACID_EXPORT OpusSoundBuffer : public SoundBuffer::Registrar<OpusSoundBuffer> {
	inline static const bool Registered = Register(".opus");
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};
}
