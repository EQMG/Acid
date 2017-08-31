#include "Audio.hpp"

namespace Flounder
{
	std::vector<Sound*> Audio::m_sounds = std::vector<Sound*>();

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
		for (int i = 0; i < m_sounds.size(); i++)
		{
			delete m_sounds[i];
		}

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_alContext);
		alcCloseDevice(m_alDevice);
	}

	void Audio::Update()
	{
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

#if FLOUNDER_VERBOSE
		printf("-- Loading: '%s' --\n", path.c_str());
		printf("Size: %i bytes\n", result.size);

		switch (result.formatTag)
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

		printf("Channels: %i\n", result.channels);
		printf("Samples Per Second: %i\n", result.samplesPerSec);
		printf("Average bytes per second: %i\n", result.averageBytesPerSec);
		printf("Block align: %i\n", result.blockAlign);
		printf("Bit per sample: %i\n", result.bitsPerSample);
		printf("-- Done --\n");
#endif

		return result;
	}

	Sound *Audio::AddSound(Sound *object)
	{
		m_sounds.push_back(object);
		return object;
	}

	Sound *Audio::GetSound(const std::string &name)
	{
		for (auto object : m_sounds)
		{
			if (object->GetName() == name)
			{
				return object;
			}
		}

		return nullptr;
	}
}
