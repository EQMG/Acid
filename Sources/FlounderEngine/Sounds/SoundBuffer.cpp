#include "SoundBuffer.hpp"

#include "../Devices/Audio.hpp"

namespace Flounder
{
	SoundBuffer::SoundBuffer(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_buffer(0)
	{
		SoundSourceInfo sourceInfo = Audio::LoadFileWav(filename);

		alGenBuffers(1, &m_buffer);
		alBufferData(m_buffer, (sourceInfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sourceInfo.data, sourceInfo.size, sourceInfo.samplesPerSec);

		delete[] sourceInfo.data;

		Platform::ErrorAl(alGetError());
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(1, &m_buffer);
	}
}
