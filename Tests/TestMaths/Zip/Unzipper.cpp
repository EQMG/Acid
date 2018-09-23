#include "Unzipper.hpp"

#include <cassert>
#include <mz.h>
#include <mz_compat.h>
#include <fstream>
#include <Helpers/String.hpp>
#include "Engine/Log.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
#define WRITEBUFFERSIZE 8192

	Unzipper::Unzipper(const std::string &filename, const std::string &password) :
		m_filename(filename),
		m_password(password),
		m_zipFile(nullptr)
	{
	}

	void Unzipper::Open()
	{
		m_zipFile = unzOpen64(m_filename.c_str());
		assert(m_zipFile != nullptr && "Failed to open the zip file!");
	}

	void Unzipper::Close()
	{
		unzClose(m_zipFile);
	}

	void Unzipper::StreamEntry(const std::string &path, std::ostream &stream)
	{
		int err = unzLocateFile(m_zipFile, path.c_str(), NULL);

		if (err != UNZ_OK)
		{
			return;
		}

		err = unzOpenCurrentFile(m_zipFile);

		ZipEntry entry = CurrentEntryInfo();

		std::vector<char> buffer;
		buffer.resize(WRITEBUFFERSIZE);

		err = unzReadCurrentFile(m_zipFile, buffer.data(), (unsigned int) buffer.size());

		//if (err < 0 || err == 0)
		//{
		//	break;
		//}

		stream.write(buffer.data(), err);

		stream.flush();
		err = unzCloseCurrentFile(m_zipFile);
	}

	void Unzipper::Extract(const std::string &path)
	{
		std::vector<ZipEntry> entries = GetEntries();

		for (auto &entry : entries)
		{
			std::string filename = path + "/" + entry.GetName();
			FileSystem::Create(filename);

			if (!FileSystem::IsFile(filename))
			{
				continue;
			}

			std::ofstream fileWrite;
			fileWrite.open(filename);
			StreamEntry(entry.GetName(), fileWrite);
			fileWrite.close();
		}
	}

	std::vector<ZipEntry> Unzipper::GetEntries() const
	{
		std::vector<ZipEntry> entries = {};

		int err = unzGoToFirstFile(m_zipFile);

		if (UNZ_OK == err)
		{
			do
			{
				ZipEntry entryInfo = CurrentEntryInfo();

				if (entryInfo.IsValid())
				{
					entries.push_back(entryInfo);
					err = unzGoToNextFile(m_zipFile);
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
		return UNZ_OK == unzLocateFile(m_zipFile, name.c_str(), nullptr);
	}

	ZipEntry Unzipper::CurrentEntryInfo() const
	{
		unz_file_info64 file_info = { 0 };
		char filename_inzip[256] = { 0 };

		int err = unzGetCurrentFileInfo64(m_zipFile, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
		assert(UNZ_OK == err && "Error, could not get the current entry info!");

		return ZipEntry(std::string(filename_inzip), file_info.compressed_size, file_info.uncompressed_size,
		                file_info.tmu_date.tm_year, file_info.tmu_date.tm_mon, file_info.tmu_date.tm_mday,
		                file_info.tmu_date.tm_hour, file_info.tmu_date.tm_min, file_info.tmu_date.tm_sec, file_info.dosDate);
	}
}
