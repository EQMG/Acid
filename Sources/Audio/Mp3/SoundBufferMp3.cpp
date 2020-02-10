#include "SoundBufferMp3.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include <dr_libs/dr_mp3.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

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

	drmp3_config config;
	drmp3_uint64 totalPCMFrameCount;
	auto sampleData = drmp3_open_memory_and_read_pcm_frames_s16(fileLoaded->data(), fileLoaded->size(), &config, &totalPCMFrameCount, nullptr);
	if (!sampleData) {
		Log::Error("Error reading OGG ", filename, ", could not load samples\n");
		return;
	}

	uint32_t buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, (config.outputChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sampleData, totalPCMFrameCount, config.outputSampleRate);

	Audio::CheckAl(alGetError());

	soundBuffer->SetBuffer(buffer);

	drmp3_free(sampleData, nullptr);

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
