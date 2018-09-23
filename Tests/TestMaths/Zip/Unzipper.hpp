#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace acid
{
	class ZipEntry
	{
	private:
		typedef struct
		{
			uint32_t m_second;
			uint32_t m_minute;
			uint32_t m_hour;
			uint32_t m_day;
			uint32_t m_month;
			uint32_t m_year;
		} tm_s;

		std::string m_name;
		std::string m_timestamp;
		uint64_t m_compressedSize;
		uint64_t m_uncompressedSize;
		uint32_t m_dosDate;
		tm_s m_unixDate;
	public:
		ZipEntry(const std::string& name, const uint64_t &compressedSize, const uint64_t &uncompressedSize,
		         const int32_t &year, const int32_t &month, const int32_t &day, const int32_t &hour, const int32_t &minute, const int32_t &second,
		         const uint32_t &dosDate) :
			m_name(name),
			m_compressedSize(compressedSize),
			m_uncompressedSize(uncompressedSize),
			m_dosDate(dosDate),
			m_unixDate({})
		{
			// Timestamp: YYYY-MM-DD HH:MM:SS
			std::stringstream str;
			str << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second;
			m_timestamp = str.str();

			m_unixDate.m_year = static_cast<uint32_t>(year);
			m_unixDate.m_month = static_cast<uint32_t>(month);
			m_unixDate.m_day = static_cast<uint32_t>(day);
			m_unixDate.m_hour = static_cast<uint32_t>(hour);
			m_unixDate.m_minute = static_cast<uint32_t>(minute);
			m_unixDate.m_second = static_cast<uint32_t>(second);
		}

		bool IsValid() const { return !m_name.empty(); }

		std::string GetName() const { return m_name; }

		std::string GetTimestamp() const { return m_timestamp; }

		uint64_t GetCompressedSize() const { return m_compressedSize; }

		uint64_t GetUncompressedSize() const { return m_uncompressedSize; }

		uint32_t GetDosDate() const { return m_dosDate; }

		tm_s GetUnixDate() const { return m_unixDate; }
	};

	typedef void *zipFile;
	typedef void *zlib_filefunc_def;

	class Unzipper
	{
	private:
		std::string m_filename;
		std::string m_password;

		zipFile m_zf;
		zlib_filefunc_def m_filefunc;
	public:
		explicit Unzipper(const std::string &filename, const std::string &password = "");

		void Open();

		void Close();

		std::vector<ZipEntry> GetEntries() const;

		std::string GetFilename() const { return m_filename; };
	private:
		bool LocateEntry(const std::string& name);

		ZipEntry CurrentEntryInfo() const;
	};
}
