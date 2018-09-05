#pragma once

#include <string>
#include <vector>
#include "Files/Files.hpp"
#include "Maths/Vector3.hpp"
#include "Resources/IResource.hpp"
#include "Audio.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a sound buffer.
	/// </summary>
	class ACID_EXPORT SoundBuffer :
		public IResource
	{
	private:
		std::string m_filename;
		uint32_t m_buffer;
	public:
		/// <summary>
		/// Will find an existing sound buffer with the same filename, or create a new sound buffer.
		/// </summary>
		/// <param name="filename"> The file to load the sound buffer from. </param>
		static std::shared_ptr<SoundBuffer> Resource(const std::string &filename);

		/// <summary>
		/// Creates a new sound buffer.
		/// </summary>
		/// <param name="filename"> The file to load the sound buffer from. </param>
		SoundBuffer(const std::string &filename);

		~SoundBuffer();

		std::string GetFilename() override { return m_filename; };

		uint32_t GetBuffer() const { return m_buffer; };

	private:
		static uint32_t LoadBufferWav(const std::string &filename);

		static uint32_t LoadBufferOgg(const std::string &filename);
	};
}
