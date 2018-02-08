#include "SoundBuffer.hpp"

#include "../Helpers/FileSystem.hpp"

namespace Flounder
{
	SoundBuffer::SoundBuffer(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_buffer(0)
	{
		if (FileSystem::FindExt(filename) == "wav")
		{
			m_buffer = Audio::LoadFileWav(filename);
		}
		else if (FileSystem::FindExt(filename) == "ogg")
		{
			m_buffer = Audio::LoadFileOgg(filename);
		}

		Platform::ErrorAl(alGetError());
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(1, &m_buffer);
	}
}
