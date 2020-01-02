#include "SoundBufferWave.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_STDIO
#include "dr_wav.h"

namespace acid {
bool SoundBufferWave::registered = Register(".wav", ".wave");

void SoundBufferWave::Load(SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("SoundBuffer could not be loaded: ", filename, '\n');
		return;
	}

	std::stringstream file;
	file << *fileLoaded;

	char chunkId[5] = "\0";

	// Read header.
	uint32_t size;

	file.read(chunkId, 4);
	file.read(reinterpret_cast<char *>(&size), 4);

	chunkId[4] = '\0';
	file.read(chunkId, 4);

	chunkId[4] = '\0';

	// Read first chunk header.
	file.read(chunkId, 4);
	file.read(reinterpret_cast<char *>(&size), 4);

	chunkId[4] = '\0';

	// Read first chunk content.
	int16_t formatTag;
	int16_t channels;
	int32_t samplesPerSec;
	int32_t averageBytesPerSec;
	int16_t blockAlign;
	int16_t bitsPerSample;

	file.read(reinterpret_cast<char *>(&formatTag), 2);
	file.read(reinterpret_cast<char *>(&channels), 2);
	file.read(reinterpret_cast<char *>(&samplesPerSec), 4);
	file.read(reinterpret_cast<char *>(&averageBytesPerSec), 4);
	file.read(reinterpret_cast<char *>(&blockAlign), 2);
	file.read(reinterpret_cast<char *>(&bitsPerSample), 2);

	if (size > 16) {
		file.seekg(static_cast<int>(file.tellg()) + (size - 16));
	}

	// Read data chunk header.
	file.read(chunkId, 4);
	file.read(reinterpret_cast<char *>(&size), 4);

	chunkId[4] = '\0';

	std::unique_ptr<uint8_t[]> data(new uint8_t[size]);
	file.read(reinterpret_cast<char *>(data.get()), size);

	uint32_t buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data.get(), size, samplesPerSec);

	Audio::CheckAl(alGetError());

	soundBuffer->SetBuffer(buffer);
	
#if defined(ACID_DEBUG)
	Log::Out("SoundBuffer ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void SoundBufferWave::Write(const SoundBuffer *soundBuffer, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	// TODO: Implement

#if defined(ACID_DEBUG)
	Log::Out("SoundBuffer ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
