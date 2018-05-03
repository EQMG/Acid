#include "Audio.hpp"

#include <fstream>
//#ifdef FL_BUILD_WINDOWS
//#include <Windows.h>
//#endif
#include "Helpers/FileSystem.hpp"
#include "Scenes/Scenes.hpp"

namespace fl
{
	Audio::Audio() :
		IModule(),
		m_alDevice(nullptr),
		m_alContext(nullptr)
	{
		m_alDevice = alcOpenDevice(nullptr);
		m_alContext = alcCreateContext(m_alDevice, nullptr);
		alcMakeContextCurrent(m_alContext);
	}

	Audio::~Audio()
	{
		alcMakeContextCurrent(nullptr);
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
			ALfloat orientation[6] = {currentRay->m_x, currentRay->m_y, currentRay->m_z, 0.0f, 1.0f, 0.0f};

			alListenerfv(AL_ORIENTATION, orientation);
			ErrorAl(alGetError());
		}
	}

	void Audio::ErrorAl(const ALenum &result)
	{
		if (result == AL_NO_ERROR)
		{
			return;
		}

		fprintf(stderr, "OpenAL error: %i\n", result);
//#ifdef FL_BUILD_WINDOWS
//		MessageBox(nullptr, "" + result, "OpenAL Error", 0);
//#endif
		throw std::runtime_error("OpenAL runtime error.");
	}

	ALuint Audio::LoadFileWav(const std::string &filename)
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

	ALuint Audio::LoadFileOgg(const std::string &filename)
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
