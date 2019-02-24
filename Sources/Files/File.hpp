#pragma once

#include <memory>
#include <string>
#include "Engine/Exports.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	/// <summary>
	/// A class that represents a readable and writable file format using <seealso cref="Metadata"/> as storage.
	/// </summary>
	class ACID_EXPORT File
	{
	public:
		explicit File(std::string filename, Metadata *metadata);

		void Read();

		void Write();

		void Clear();

		const std::string &GetFilename() const { return m_filename; }

		void SetFilename(const std::string &filename) { m_filename = filename; }

		Metadata *GetMetadata() const { return m_metadata.get(); }
	private:
		void Verify();

		std::string m_filename;
		std::unique_ptr<Metadata> m_metadata;
	};
}
