#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include <miniz/miniz.h>

#include "Utils/NonCopyable.hpp"

namespace acid {
using ZipEntryInfo = mz_zip_archive_file_stat;
using ZipEntryData = std::vector<std::byte>;

class ACID_EXPORT ZipEntryMetaData {
public:
	explicit ZipEntryMetaData(const ZipEntryInfo &info);

	uint32_t index;
	uint64_t compressedSize;
	uint64_t uncompressedSize;
	bool isDirectory;
	bool isEncrypted;
	bool isSupported;
	std::string filename;
	std::string comment;
	const time_t time;
};

class ACID_EXPORT ZipEntry : NonCopyable {
	friend class ZipArchive;
public:
	/**
	 * @brief Constructor. Creates a new ZipEntry with the given ZipEntryInfo parameter. This is only used for creating
	 * a ZipEntry for an entry already present in the ZipArchive.
	 * @param info
	 */
	explicit ZipEntry(const ZipEntryInfo &info);

	/**
	 * @brief Constructor. Creates a new ZipEntry with the given name and binary data. This should only be used for creating
	 * new entries, not already present in the ZipArchive
	 * @param name
	 * @param data
	 */
	ZipEntry(const std::string &name, const ZipEntryData &data);

	/**
	 * @brief Constructor. Creates a new ZipEntry with the given name and string data. This should only be used for creating
	 * new entries, not already present in the ZipArchive
	 * @param name
	 * @param data
	 */
	ZipEntry(const std::string &name, const std::string &data);

	virtual ~ZipEntry() = default;

	ZipEntryData GetData() const { return entryData; }

	std::string GetDataAsString() const;

	void SetData(const std::string &data) {
		ZipEntryData result;

		for (auto &ch : data)
			result.push_back(static_cast<std::byte>(ch));

		entryData = result;
		isModified = true;
	}

	void SetData(const ZipEntryData &data);

	std::string GetName() const {
		// TODO: Implement
		return std::string();
	}

	void SetName(const std::string &name) {
		// TODO: Implement
	}

	uint32_t GetIndex() const { return entryInfo.m_file_index; }
	uint64_t GetCompressedSize() const { return entryInfo.m_comp_size; }
	uint64_t GetUncompressedSize() const { return entryInfo.m_uncomp_size; }
	bool IsDirectory() const { return entryInfo.m_is_directory; }
	bool IsEncrypted() const { return entryInfo.m_is_encrypted; }
	bool IsSupported() const { return entryInfo.m_is_supported; }
	std::string GetFilename() const { return entryInfo.m_filename; }
	std::string GetComment() const { return entryInfo.m_comment; }
	const time_t &GetTime() const { return entryInfo.m_time; }

private:
	bool IsModified() const { return isModified; }

	static uint32_t GetNewIndex(uint32_t latestIndex = 0);
	static ZipEntryInfo CreateInfo(const std::string &name);

	ZipEntryInfo entryInfo = {};
	ZipEntryData entryData;
	bool isModified = false;
};
}
