#include "OpusSoundBuffer.hpp"

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include <dr_libs/dr_opus.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

namespace acid {
void OpusSoundBuffer::Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
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

void OpusSoundBuffer::Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
