#pragma once

#include "Maths/Vector3.hpp"
#include "Resources/Resource.hpp"
#include "Audio.hpp"

namespace acid
{
/**
 * @brief Resource that represents a sound buffer.
 */
class SoundBuffer :
	public Resource
{
public:
	/**
	 * Creates a new sound buffer, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The sound buffer with the requested values.
	 */
	static std::shared_ptr<SoundBuffer> Create(const Metadata &metadata);

	/**
	 * Creates a new sound buffer, or finds one with the same values.
	 * @param filename The file to load the sound buffer from.
	 * @return The sound buffer with the requested values.
	 */
	static std::shared_ptr<SoundBuffer> Create(const std::string &filename);

	/**
	 * Creates a new sound buffer.
	 * @param filename The file to load the sound buffer from.
	 * @param load If this resource will be loaded immediately, otherwise {@link SoundBuffer#Load} can be called later.
	 */
	explicit SoundBuffer(std::string filename, const bool &load = true);

	~SoundBuffer();

	void Load() override;

	const std::string &GetFilename() const { return m_filename; };

	const uint32_t &GetBuffer() const { return m_buffer; }

	friend const Metadata &operator>>(const Metadata &metadata, SoundBuffer &soundBuffer);

	friend Metadata &operator<<(Metadata &metadata, const SoundBuffer &soundBuffer);

private:
	static uint32_t LoadBufferWav(const std::string &filename);

	static uint32_t LoadBufferOgg(const std::string &filename);

	std::string m_filename;
	uint32_t m_buffer;
};
}
