#pragma once

#include <string>
#include <vector>
#include "Audio/Audio.hpp"
#include "Resources/Resources.hpp"
#include "Maths/Vector3.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a sound buffer.
	/// </summary>
	class FL_EXPORT SoundBuffer :
		public IResource
	{
	private:
		std::string m_filename;
		ALuint m_buffer;
	public:
		static SoundBuffer *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<SoundBuffer *>(resource);
			}

			SoundBuffer *result = new SoundBuffer(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		SoundBuffer(const std::string &filename);

		~SoundBuffer();

		std::string GetFilename() override { return m_filename; };

		ALuint GetBuffer() const { return m_buffer; };
	};
}
