#include "SoundBuffer.hpp"

#ifdef FL_BUILD_MACOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
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
			m_buffer = Audio::LoadFileWav(filename);
		}
		else if (FileSystem::FindExt(filename) == "ogg")
		{
			m_buffer = Audio::LoadFileOgg(filename);
		}

		Audio::ErrorAl(alGetError());
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers(1, &m_buffer);
	}
}
