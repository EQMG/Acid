#include "SoundBuffer.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "stb_vorbis.c"

namespace acid
{
std::shared_ptr<SoundBuffer> SoundBuffer::Create(const Node &node)
{
	if (auto resource = Resources::Get()->Find(node))
	{
		return std::dynamic_pointer_cast<SoundBuffer>(resource);
	}

	auto result = std::make_shared<SoundBuffer>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<SoundBuffer> SoundBuffer::Create(const std::filesystem::path &filename)
{
	SoundBuffer temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

SoundBuffer::SoundBuffer(std::filesystem::path filename, bool load) :
	m_filename(std::move(filename))
{
	if (load)
	{
		Load();
	}
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &m_buffer);
}

const Node &operator>>(const Node &node, SoundBuffer &soundBuffer)
{
	node["filename"].Get(soundBuffer.m_filename);
	return node;
}

Node &operator<<(Node &node, const SoundBuffer &soundBuffer)
{
	node["filename"].Set(soundBuffer.m_filename);
	return node;
}

void SoundBuffer::Load()
{
	if (m_filename.empty())
	{
		return;
	}

	auto fileExt = m_filename.extension().string();

	if (fileExt == ".wav")
	{
		m_buffer = LoadBufferWav(m_filename);
	}
	else if (fileExt == ".ogg")
	{
		m_buffer = LoadBufferOgg(m_filename);
	}
}

uint32_t SoundBuffer::LoadBufferWav(const std::filesystem::path &filename)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded)
	{
		Log::Error("WAV file could not be loaded: ", filename, '\n');
		return 0;
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

	if (size > 16)
	{
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

#if defined(ACID_VERBOSE)
	Log::Out("Sound WAV ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
	return buffer;
}

uint32_t SoundBuffer::LoadBufferOgg(const std::filesystem::path &filename)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded)
	{
		Log::Error("OGG file could not be loaded: ", filename, '\n');
		return 0;
	}

	int32_t channels;
	int32_t samplesPerSec;
	int16_t *data;
	auto size = stb_vorbis_decode_memory(reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()), &channels, &samplesPerSec, &data);

	if (size == -1)
	{
		Log::Error("Error reading the OGG ", filename, ", could not find size\n");
	}

	uint32_t buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

	delete[] data;
	Audio::CheckAl(alGetError());

#if defined(ACID_VERBOSE)
	Log::Out("Sound OGG ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
	return buffer;
}
}
