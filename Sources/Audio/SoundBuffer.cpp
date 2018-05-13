#include "SoundBuffer.hpp"

#include <fstream>
#ifdef FL_BUILD_MACOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <STB/stb_vorbis.h>
#include "Helpers/FileSystem.hpp"

namespace fl
{
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

		Audio::ErrorAl(alGetError());
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(1, &m_buffer);
	}

	unsigned int SoundBuffer::LoadBufferWav(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			return 0;
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Load wav file failure: file couldn't be opened!");
		}

		char chunkId[5] = "\0";

		// Read header.
		unsigned int size;

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
		int samplesPerSec;
		int averageBytesPerSec;
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

		unsigned char *data = new unsigned char[size];
		file.read(reinterpret_cast<char *>(data), size);

		file.close();

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

		delete[] data;

		return buffer;
	}

	unsigned int SoundBuffer::LoadBufferOgg(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);
		//	SoundSourceInfo sourceInfo = {};

		if (!file.is_open())
		{
			throw std::runtime_error("Load wav file failure: file couldn't be opened!");
		}

		int channels;
		int samplesPerSec;
		short *data;
		int size = stb_vorbis_decode_filename(filename.c_str(), &channels, &samplesPerSec, &data);

		if (size == -1)
		{
			fprintf(stderr, "Error reading the OGG '%s', could not find size! The audio could not be loaded.\n", filename.c_str());
		}

		file.close();
		//	LogOpenAlSound(filename, sourceInfo);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

		delete[] data;

		return buffer;
	}
}
