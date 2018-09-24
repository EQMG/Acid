#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Engine/Exports.hpp"

typedef struct mz_zip_archive_tag mz_zip_archive;

namespace acid
{
	struct ZipEntry
	{
		std::string filename;

		struct
		{
			int year = 1980;
			int month = 0;
			int day = 0;
			int hours = 0;
			int minutes = 0;
			int seconds = 0;
		} date_time;

		std::string comment;
		std::string extra;
		uint16_t create_system = 0;
		uint16_t create_version = 0;
		uint16_t extract_version = 0;
		uint16_t flag_bits = 0;
		std::size_t volume = 0;
		uint32_t internal_attr = 0;
		uint32_t external_attr = 0;
		std::size_t header_offset = 0;
		uint32_t crc = 0;
		std::size_t compress_size = 0;
		std::size_t file_size = 0;
	};

	class ACID_EXPORT FileZip
	{
	private:
		std::string m_comment;

		std::unique_ptr<mz_zip_archive> m_archive;
		std::vector<char> m_buffer;
		std::stringstream m_openStream;
		std::string m_filename;
	public:
		FileZip();

		explicit FileZip(const std::string &filename);

		explicit FileZip(std::istream &stream);

		explicit FileZip(const std::vector<unsigned char> &bytes);

		FileZip(const FileZip&) = delete;

		~FileZip();

		FileZip& operator=(const FileZip&) = delete;

		void Load(std::istream &stream);

		void Load(const std::string &filename);

		void Load(const std::vector<unsigned char> &bytes);

		void Save(const std::string &filename);

		void Save(std::ostream &stream);

		void Save(std::vector<unsigned char> &bytes);

		void Reset();

		bool HasFile(const std::string &name);

		bool HasFile(const ZipEntry &name);

		ZipEntry GetEntry(const std::string &name);

		std::vector<ZipEntry> GetEntries();

		std::vector<std::string> GetNames();

		std::ostream &Open(const std::string &name);

		std::ostream &Open(const ZipEntry &name);

		void Extract(const std::string &member, const std::string &path);

		void Extract(const ZipEntry &member, const std::string &path);

		void ExtractAll(const std::string &path);

		void ExtractAll(const std::string &path, const std::vector<std::string> &members);

		void ExtractAll(const std::string &path, const std::vector<ZipEntry> &members);

		std::string Read(const ZipEntry &info);

		std::string Read(const std::string &name);

		void Write(const std::string &filename);

		void Write(const std::string &filename, const std::string &arcname);

		void WriteAll(const std::string &path);

		void WriteString(const std::string &arcname, const std::string &bytes);

		void WriteString(const ZipEntry &info, const std::string &bytes);

		void PrintDir();

		void PrintDir(std::ostream &stream);

		std::pair<bool, std::string> TestZip();

		std::string GetFilename() const { return m_filename; }
	private:
		void StartRead();

		void StartWrite();

		void AppendComment();

		void RemoveComment();

		ZipEntry GetEntry(int index);

		static uint32_t Crc32buf(const char *buf, std::size_t len);

		static tm SafeLocaltime(const time_t &t);
		
		static std::size_t WriteCallback(void *opaque, unsigned long long file_ofs, const void *pBuf, std::size_t n);
	};
}
