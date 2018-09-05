#include "SoundBuffer.hpp"

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#include <fstream>
#include "Helpers/FileSystem.hpp"
#include "Resources/Resources.hpp"
#include "stb_vorbis.h"

namespace acid
{
	std::shared_ptr<SoundBuffer> SoundBuffer::Resource(const std::string &filename)
	{
		std::string realFilename = Files::SearchFile(filename);
		auto resource = Resources::Get()->Get(realFilename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<SoundBuffer>(resource);
		}

		auto result = std::make_shared<SoundBuffer>(realFilename);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	SoundBuffer::SoundBuffer(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_buffer(0)
	{
		if (FileSystem::FindExt(filename) == "wav")
		{
			m_buffer = LoadBufferWav(filename);
		}
		else if (FileSystem::FindExt(filename) == "ogg")
		{
			m_buffer = LoadBufferOgg(filename);
		}

		Audio::CheckAl(alGetError());
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(1, &m_buffer);
	}

	uint32_t SoundBuffer::LoadBufferWav(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			Log::Error("File does not exist: '%s'\n", filename.c_str());
			return 0;
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Load wav file failure: file couldn't be opened!");
		}

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
		short formatTag;
		short channels;
		int32_t samplesPerSec;
		int32_t averageBytesPerSec;
		short blockAlign;
		short bitsPerSample;

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

		std::unique_ptr<unsigned char[]> data(new unsigned char[size]);
		file.read(reinterpret_cast<char *>(data.get()), size);

		file.close();

		uint32_t buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data.get(), size, samplesPerSec);

		return buffer;
	}

	uint32_t SoundBuffer::LoadBufferOgg(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			Log::Error("File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Load wav file failure: file couldn't be opened!");
		}

		int32_t channels;
		int32_t samplesPerSec;
		short *data;
		int32_t size = stb_vorbis_decode_filename(filename.c_str(), &channels, &samplesPerSec, &data);

		if (size == -1)
		{
			Log::Error("Error reading the OGG '%s', could not find size! The audio could not be loaded.\n", filename.c_str());
		}

		file.close();

		uint32_t buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

		delete[] data;

		return buffer;
	}
}
