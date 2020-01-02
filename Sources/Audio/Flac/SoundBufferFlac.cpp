#include "SoundBufferFlac.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_STDIO
#define DR_FLAC_NO_SIMD
#include "dr_flac.h"

namespace acid {
bool SoundBufferFlac::registered = Register(".flac");

void SoundBufferFlac::Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
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

void SoundBufferFlac::Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#if defined(ACID_DEBUG)
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
