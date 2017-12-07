#include "Audio.hpp"

#include <fstream>
#include "../Camera/Camera.hpp"

namespace Flounder
{
	Audio::Audio() :
		IModule()
	//	m_alDevice(nullptr),
	//	m_alContext(nullptr)
	{
	//	m_alDevice = alcOpenDevice(NULL);
	//	m_alContext = alcCreateContext(m_alDevice, NULL);
	//	alcMakeContextCurrent(m_alContext);
	}

	Audio::~Audio()
	{
	//	alcMakeContextCurrent(NULL);
	//	alcDestroyContext(m_alContext);
	//	alcCloseDevice(m_alDevice);
	}

	void Audio::Update()
	{
		if (Camera::Get() == nullptr)
		{
			return;
		}

		ICamera *camera = Camera::Get()->GetCamera();

		if (camera != nullptr)
		{
			// Listener position.
		//	alListener3f(AL_POSITION, camera->GetPosition()->m_x, camera->GetPosition()->m_y, camera->GetPosition()->m_z);

			// Listener velocity.
		//	alListener3f(AL_VELOCITY, camera->GetVelocity()->m_x, camera->GetVelocity()->m_y, camera->GetVelocity()->m_z);

			// Listener orientation.
			Vector3 *currentRay = camera->GetViewRay()->m_currentRay;
			float orientation[6] = { currentRay->m_x, currentRay->m_y, currentRay->m_z, 0.0f, 1.0f, 0.0f };

		//	alListenerfv(AL_ORIENTATION, orientation);

		//	Platform::ErrorAl(alGetError());
		}
	}

	SoundSourceInfo Audio::LoadFileWav(const std::string &path)
	{
		std::ifstream file(path.c_str(), std::ifstream::binary);
		SoundSourceInfo result = {};

		if (!file.is_open())
		{
			throw std::runtime_error("Load wave file failure: file couldn't be opened!");
		}

		char chunkId[5] = "\0";

		// Read header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char*>(&result.size), 4);

		chunkId[4] = '\0';
		file.read(chunkId, 4);

		chunkId[4] = '\0';

		// Read first chunk header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char*>(&result.size), 4);

		chunkId[4] = '\0';

		// Read first chunk content.
		file.read(reinterpret_cast<char*>(&result.formatTag), 2);
		file.read(reinterpret_cast<char*>(&result.channels), 2);
		file.read(reinterpret_cast<char*>(&result.samplesPerSec), 4);
		file.read(reinterpret_cast<char*>(&result.averageBytesPerSec), 4);
		file.read(reinterpret_cast<char*>(&result.blockAlign), 2);
		file.read(reinterpret_cast<char*>(&result.bitsPerSample), 2);

		if (result.size > 16)
		{
			file.seekg(static_cast<int>(file.tellg()) + (result.size - 16));
		}

		// Read data chunk header.
		file.read(chunkId, 4);
		file.read(reinterpret_cast<char*>(&result.size), 4);

		chunkId[4] = '\0';

		result.data = new unsigned char[result.size];
		file.read(reinterpret_cast<char*>(result.data), result.size);

		file.close();
		LogOpenAlSound(path, result);
		return result;
	}

	SoundSourceInfo Audio::LoadFileOgg(const std::string &path)
	{
		std::ifstream file(path.c_str(), std::ifstream::binary);
		SoundSourceInfo result = {};

		if (!file.is_open())
		{
			throw std::runtime_error("Load wave file failure: file couldn't be opened!");
		}

		// TODO

		file.close();
		LogOpenAlSound(path, result);
		return result;
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
