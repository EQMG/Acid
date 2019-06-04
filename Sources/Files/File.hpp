#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
/**
 * @brief Class that represents a readable and writable file format using {@link Metadata} as storage.
 */
class ACID_EXPORT File
{
public:
	explicit File(std::string filename, Metadata *metadata);

	void Load();

	void Write() const;

	void Clear();

	const std::string &GetFilename() const { return m_filename; }

	void SetFilename(const std::string &filename) { m_filename = filename; }

	Metadata *GetMetadata() const { return m_metadata.get(); }

private:
	std::string m_filename;
	std::unique_ptr<Metadata> m_metadata;
};
}
