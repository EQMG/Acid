#include "SoundBufferMp3.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

#define DR_MP3_IMPLEMENTATION
#define DR_MP3_NO_STDIO
#define DR_MP3_NO_SIMD
#include "dr_mp3.h"

namespace acid {
bool SoundBufferMp3::registered = Register(".mp3");

void SoundBufferMp3::Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("SoundBuffer could not be loaded: ", filename, '\n');
		return;
	}

	//soundBuffer->SetBuffer(buffer);

#if defined(ACID_DEBUG)
	Log::Out("SoundBuffer ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void SoundBufferMp3::Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#if defined(ACID_DEBUG)
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
