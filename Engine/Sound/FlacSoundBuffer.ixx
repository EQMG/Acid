module;

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include <dr_flac.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

export module acid.soundbuffer.flac;
import acid.soundbuffer;
import acid.sound;

export namespace acid {
class FlacSoundBuffer : public SoundBuffer::Registrar<FlacSoundBuffer> {
	inline static const bool Registered = Register(".flac");
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};

void FlacSoundBuffer::Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("SoundBuffer could not be loaded: ", filename, '\n');
		return;
	}

	//soundBuffer->SetBuffer(buffer);

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void FlacSoundBuffer::Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
