#include "audio.h"

namespace flounder
{
	std::vector<sound*> audio::m_sounds = std::vector<sound*>();

	audio::audio() :
		imodule()
	{
		m_device = alcOpenDevice(NULL);
		m_context = alcCreateContext(m_device, NULL);
		alcMakeContextCurrent(m_context);
	}

	audio::~audio()
	{
		for (int i = 0; i < m_sounds.size(); i++)
		{
			delete m_sounds[i];
		}

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	void audio::update()
	{
	}

	SoundSourceInfo audio::loadWaveFile(const std::string path)
	{
		std::ifstream file(path.c_str(), std::ifstream::binary);
		SoundSourceInfo result = {};

		if (!file.is_open())
		{
			throw std::runtime_error("Load wave file failure: file couldn't be opened!");
		}
		else
		{
			char chunkId[5] = "\0";

			// Read header.
			file.read(chunkId, 4);
			file.read((char*) &result.size, 4);

			chunkId[4] = '\0';
			file.read(chunkId, 4);

			chunkId[4] = '\0';

			// Read first chunk header.
			file.read(chunkId, 4);
			file.read((char*) &result.size, 4);

			chunkId[4] = '\0';

			// Read first chunk content.
			file.read((char*) &result.formatTag, 2);
			file.read((char*) &result.channels, 2);
			file.read((char*) &result.samplesPerSec, 4);
			file.read((char*) &result.averageBytesPerSec, 4);
			file.read((char*) &result.blockAlign, 2);
			file.read((char*) &result.bitsPerSample, 2);

			if (result.size > 16)
			{
				file.seekg((int) file.tellg() + (result.size - 16));
			}

			// Read data chunk header.
			file.read(chunkId, 4);
			file.read((char*) &result.size, 4);

			chunkId[4] = '\0';

			result.data = new unsigned char[result.size];
			file.read((char*) result.data, result.size);
		}

		file.close();

#ifdef 0 
		std::cout << "-- Loading: " << path << " --" << std::endl;
		std::cout << "Size: " << result.size << " bytes" << std::endl;

		switch (result.formatTag)
		{
		case 0x0001:
			std::cout << "Format: PCM" << std::endl;
			break;
		case 0x0003:
			std::cout << "Format: IEEE Float" << std::endl;
			break;
		case 0x0006:
			std::cout << "Format: 8-bit ITU-T G.711 A-law" << std::endl;
			break;
		case 0x0007:
			std::cout << "Format: 8-bit ITU-T G.711 mi-law" << std::endl;
			break;
		default:
			std::cout << "Format: Unknown tag" << std::endl;
			break;
		}

		std::cout << "Channels: " << result.channels << std::endl;
		std::cout << "Samples Per Second: " << result.samplesPerSec << std::endl;
		std::cout << "Average bytes per second: " << result.averageBytesPerSec << std::endl;
		std::cout << "Block align: " << result.blockAlign << std::endl;
		std::cout << "Bit per sample: " << result.bitsPerSample << std::endl;
		std::cout << "-- Done --" << std::endl << std::endl;
#endif

		return result;
	}

	sound *audio::add(sound *object)
	{
		m_sounds.push_back(object);
		return object;
	}

	sound *audio::get(const std::string &name)
	{
		for (sound *object : m_sounds)
		{
			if (object->getName() == name)
			{
				return object;
			}
		}

		return nullptr;
	}
}
