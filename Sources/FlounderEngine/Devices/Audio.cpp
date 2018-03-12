#include "Audio.hpp"

#include <cassert>
#include <fstream>
#include "../Helpers/FileSystem.hpp"
#include "../Scenes/Scenes.hpp"

namespace Flounder
{
	Audio *Audio::S_INSTANCE = nullptr;

	Audio::Audio() :
		IModule(),
		m_alDevice(nullptr),
		m_alContext(nullptr)
	{
		m_alDevice = alcOpenDevice(NULL);
		m_alContext = alcCreateContext(m_alDevice, NULL);
		alcMakeContextCurrent(m_alContext);
	}

	Audio::~Audio()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_alContext);
		alcCloseDevice(m_alDevice);
	}

	void Audio::Update()
	{
		ICamera *camera = Scenes::Get()->GetCamera();

		if (camera != nullptr)
		{
			// Listener position.
			alListener3f(AL_POSITION, camera->GetPosition()->m_x, camera->GetPosition()->m_y, camera->GetPosition()->m_z);

			// Listener velocity.
			alListener3f(AL_VELOCITY, camera->GetVelocity()->m_x, camera->GetVelocity()->m_y, camera->GetVelocity()->m_z);

			// Listener orientation.
			Vector3 *currentRay = camera->GetViewRay()->m_currentRay;
			float orientation[6] = {currentRay->m_x, currentRay->m_y, currentRay->m_z, 0.0f, 1.0f, 0.0f};

			alListenerfv(AL_ORIENTATION, orientation);
			Platform::ErrorAl(alGetError());
		}
	}

	ALuint Audio::LoadFileWav(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);
		SoundSourceInfo sourceInfo = {};

		assert(file.is_open() && "Load wav file failure: file couldn't be opened!");

		char chunkId[5] = "\0";

		// Read header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char *>(&sourceInfo.size), 4);

		chunkId[4] = '\0';
		file.read(chunkId, 4);

		chunkId[4] = '\0';

		// Read first chunk header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char *>(&sourceInfo.size), 4);

		chunkId[4] = '\0';

		// Read first chunk content.
		file.read(reinterpret_cast<char *>(&sourceInfo.formatTag), 2);
		file.read(reinterpret_cast<char *>(&sourceInfo.channels), 2);
		file.read(reinterpret_cast<char *>(&sourceInfo.samplesPerSec), 4);
		file.read(reinterpret_cast<char *>(&sourceInfo.averageBytesPerSec), 4);
		file.read(reinterpret_cast<char *>(&sourceInfo.blockAlign), 2);
		file.read(reinterpret_cast<char *>(&sourceInfo.bitsPerSample), 2);

		if (sourceInfo.size > 16)
		{
			file.seekg(static_cast<int>(file.tellg()) + (sourceInfo.size - 16));
		}

		// Read data chunk header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char *>(&sourceInfo.size), 4);

		chunkId[4] = '\0';

		sourceInfo.data = new unsigned char[sourceInfo.size];
		file.read(reinterpret_cast<char *>(sourceInfo.data), sourceInfo.size);

		file.close();
		//	LogOpenAlSound(filename, sourceInfo);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, (sourceInfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sourceInfo.data, sourceInfo.size, sourceInfo.samplesPerSec);

		delete[] sourceInfo.data;

		return buffer;
	}

	ALuint Audio::LoadFileOgg(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		std::ifstream file(filename.c_str(), std::ifstream::binary);
	//	SoundSourceInfo sourceInfo = {};

		assert(file.is_open() && "Load ogg file failure: file couldn't be opened!");

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

	void Audio::LogOpenAlSound(const std::string &path, const SoundSourceInfo &sourceInfo)
	{
#if FLOUNDER_VERBOSE
		printf("-- Loading Audio: '%s' --\n", path.c_str());
		printf("Size: %i bytes\n", sourceInfo.size);

		switch (sourceInfo.formatTag)
		{
		case 0x0001:
			printf("Format: PCM\n");
			break;
		case 0x0003:
			printf("Format: IEEE Float\n");
			break;
		case 0x0006:
			printf("Format: 8-bit ITU-T G.711 A-law\n");
			break;
		case 0x0007:
			printf("Format: 8-bit ITU-T G.711 mi-law\n");
			break;
		default:
			printf("Format: Unknown tag\n");
			break;
		}

		printf("Channels: %i\n", sourceInfo.channels);
		printf("Samples Per Second: %i\n", sourceInfo.samplesPerSec);
		printf("Average bytes per second: %i\n", sourceInfo.averageBytesPerSec);
		printf("Block align: %i\n", sourceInfo.blockAlign);
		printf("Bit per sample: %i\n", sourceInfo.bitsPerSample);
		printf("-- Done --\n");
#endif
	}
}
