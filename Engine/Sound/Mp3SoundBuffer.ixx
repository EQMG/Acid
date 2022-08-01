module;

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include <dr_mp3.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

export module acid.soundbuffer.mp3;
import acid.soundbuffer;
import acid.sound;

export namespace acid {
class Mp3SoundBuffer : public SoundBuffer::Registrar<Mp3SoundBuffer> {
	inline static const bool Registered = Register(".mp3");
public:
	static void Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename);
	static void Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename);
};

void Mp3SoundBuffer::Load(SoundBuffer &soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
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
	alBufferData(buffer, (config.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sampleData, totalPCMFrameCount, config.sampleRate);

	CheckAl(alGetError());

	soundBuffer.SetBuffer(buffer);

	drmp3_free(sampleData, nullptr);

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void Mp3SoundBuffer::Write(const SoundBuffer &soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
