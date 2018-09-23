#include "Unzipper.hpp"

#include <cassert>
#include <mz.h>
#include <mz_compat.h>
#include <fstream>
#include "Engine/Log.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
#define WRITEBUFFERSIZE 8192

	Unzipper::Unzipper(const std::string &filename, const std::string &password) :
		m_filename(filename),
		m_password(password),
		m_zf(nullptr),
		m_filefunc(nullptr)
	{
	}

	void Unzipper::Open()
	{
		m_zf = unzOpen64(m_filename.c_str());
		assert(m_zf != nullptr && "Failed to open the zip file!");
	}

	void Unzipper::Close()
	{
		unzClose(m_zf);
	}

	std::vector<ZipEntry> Unzipper::GetEntries() const
	{
		std::vector<ZipEntry> entries = {};

		int err = unzGoToFirstFile(m_zf);

		if (UNZ_OK == err)
		{
			do
			{
				ZipEntry entryInfo = CurrentEntryInfo();

				if (entryInfo.IsValid())
				{
					entries.push_back(entryInfo);
					err = unzGoToNextFile(m_zf);
				}
				else
				{
					err = UNZ_ERRNO;
				}
			} while (UNZ_OK == err);

			if (UNZ_END_OF_LIST_OF_FILE != err && err != UNZ_OK)
			{
				return entries;
			}
		}

		return entries;
	}

	bool Unzipper::LocateEntry(const std::string &name)
	{
		return UNZ_OK == unzLocateFile(m_zf, name.c_str(), nullptr);
	}

	ZipEntry Unzipper::CurrentEntryInfo() const
	{
		unz_file_info64 file_info = { 0 };
		char filename_inzip[256] = { 0 };

		int err = unzGetCurrentFileInfo64(m_zf, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
		assert(UNZ_OK == err && "Error, could not get the current entry info!");

		return ZipEntry(std::string(filename_inzip), file_info.compressed_size, file_info.uncompressed_size,
		                file_info.tmu_date.tm_year, file_info.tmu_date.tm_mon, file_info.tmu_date.tm_mday,
		                file_info.tmu_date.tm_hour, file_info.tmu_date.tm_min, file_info.tmu_date.tm_sec, file_info.dosDate);
	}
}
