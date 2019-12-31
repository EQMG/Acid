#include "ZipEntry.hpp"

namespace acid {
ZipEntryMetaData::ZipEntryMetaData(const ZipEntryInfo &info) :
	index(info.m_file_index),
	compressedSize(info.m_comp_size),
	uncompressedSize(info.m_uncomp_size),
	isDirectory(info.m_is_directory),
	isEncrypted(info.m_is_encrypted),
	isSupported(info.m_is_supported),
	filename(info.m_filename),
	comment(info.m_comment),
	time(info.m_time) {
}

ZipEntry::ZipEntry(const ZipEntryInfo &info) :
	m_entryInfo(info) {
	GetNewIndex(info.m_file_index);
}

ZipEntry::ZipEntry(const std::string &name, const ZipEntryData &data) {
	m_entryInfo = CreateInfo(name);
	m_entryData = data;
	m_isModified = true;
}

ZipEntry::ZipEntry(const std::string &name, const std::string &data) {
	m_entryInfo = CreateInfo(name);
	m_entryData.reserve(data.size());
	for (auto &ch : data)
		m_entryData.emplace_back(static_cast<std::byte>(ch));
	m_isModified = true;
}

std::string ZipEntry::GetDataAsString() const {
	std::string result;
	for (auto &ch : m_entryData)
		result += static_cast<char>(ch);
	return result;
}

void ZipEntry::SetData(const ZipEntryData &data) {
	m_entryData = data;
	m_isModified = true;
}

uint32_t ZipEntry::GetNewIndex(uint32_t latestIndex) {
	static uint32_t index = 0;

	if (latestIndex > index) {
		index = latestIndex;
		return index;
	}

	return ++index;
}

ZipEntryInfo ZipEntry::CreateInfo(const std::string &name) {
	ZipEntryInfo info;
	info.m_file_index = GetNewIndex(0);
	info.m_central_dir_ofs = 0;
	info.m_version_made_by = 0;
	info.m_version_needed = 0;
	info.m_bit_flag = 0;
	info.m_method = 0;
	info.m_time = time(nullptr);
	info.m_crc32 = 0;
	info.m_comp_size = 0;
	info.m_uncomp_size = 0;
	info.m_internal_attr = 0;
	info.m_external_attr = 0;
	info.m_local_header_ofs = 0;
	info.m_comment_size = 0;
	info.m_is_directory = (name.back() == '/');
	info.m_is_encrypted = false;
	info.m_is_supported = true;

#if _MSC_VER // On MSVC, use the safe version of strcpy
	strcpy_s(info.m_filename, sizeof(info.m_filename), name.c_str());
	strcpy_s(info.m_comment, sizeof(info.m_comment), "");
#else // Otherwise, use the unsafe version as fallback :(
	strncpy(info.m_filename, name.c_str(), sizeof(info.m_filename));
	strncpy(info.m_comment, "", sizeof(info.m_comment));
#endif
	return info;
}

}
