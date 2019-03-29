#pragma once

#include "Maths/Vector3.hpp"
#include "Resources/Resource.hpp"
#include "Audio.hpp"

namespace acid
{
/// <summary>
/// Class that represents a sound buffer.
/// </summary>
class ACID_EXPORT SoundBuffer :
	public Resource
{
public:
	/// <summary>
	/// Will find an existing sound buffer with the same values, or create a new sound buffer.
	/// </summary>
	/// <param name="metadata"> The metadata to decode values from. </param>
	static std::shared_ptr<SoundBuffer> Create(const Metadata &metadata);

	/// <summary>
	/// Will find an existing sound buffer with the same values, or create a new sound buffer.
	/// </summary>
	/// <param name="filename"> The file to load the sound buffer from. </param>
	static std::shared_ptr<SoundBuffer> Create(const std::string &filename);

	/// <summary>
	/// Creates a new sound buffer.
	/// </summary>
	/// <param name="filename"> The file to load the sound buffer from. </param>
	/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
	explicit SoundBuffer(std::string filename, const bool &load = true);

	~SoundBuffer();

	void Load() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	const std::string &GetFilename() const { return m_filename; };

	const uint32_t &GetBuffer() const { return m_buffer; }

private:
	static uint32_t LoadBufferWav(const std::string &filename);

	static uint32_t LoadBufferOgg(const std::string &filename);

	std::string m_filename;
	uint32_t m_buffer;
};
}
