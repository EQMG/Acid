#include "OggSoundBuffer.hpp"

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include <stb/stb_vorbis.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

namespace acid {
void OggSoundBuffer::Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("SoundBuffer could not be loaded: ", filename, '\n');
		return;
	}

	int32_t channels;
	int32_t samplesPerSec;
	int16_t *data;
	auto size = stb_vorbis_decode_memory(reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()), &channels, &samplesPerSec, &data);

	if (size == -1) {
		Log::Error("Error reading OGG ", filename, ", could not find size\n");
		return;
	}

	uint32_t buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

	Audio::CheckAl(alGetError());

	free(data);
	soundBuffer->SetBuffer(buffer);
	
#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void OggSoundBuffer::Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#ifdef ACID_DEBUG
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
