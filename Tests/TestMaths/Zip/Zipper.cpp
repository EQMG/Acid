#include "Zipper.hpp"

#include <cassert>
#include <fstream>
#include <zlib.h>
#include <mz.h>
#include <mz_zip.h>
#include <mz_compat.h>
#include "Helpers/FileSystem.hpp"

namespace acid
{
#define WRITEBUFFERSIZE 8192

	Zipper::Zipper(const std::string &filename, const std::string &password) :
		m_filename(filename),
		m_password(password),
		m_zipFile(nullptr)
	{
	}

	void Zipper::Open()
	{
#ifdef USEWIN32IOAPI
		zlib_filefunc64_def ffunc = { 0 };
#endif

		int mode = 0;
		int flags = ZIP_APPEND;

		// Opens the zip file for output.
		if (FileSystem::Exists(m_filename))
		{
			mode = (flags & ZIP_OVERWITE) ? APPEND_STATUS_CREATE : APPEND_STATUS_ADDINZIP;
		}
		else
		{
			mode = APPEND_STATUS_CREATE;
		}

#ifdef USEWIN32IOAPI
		fill_win32_filefunc64A(&ffunc);
		m_zipFile = zipOpen2_64(m_filename.c_str(), mode, NULL, &ffunc);
#else
		m_zipFile = zipOpen64(m_filename.c_str(), mode);
#endif
	}

	void Zipper::Close()
	{
		zipClose(m_zipFile, NULL);
	}

	bool Zipper::Add(std::istream &inputStream, const std::string &nameInZip, const ZipFlags &flags)
	{
		return false;
	}

	bool Zipper::Add(const std::string &filename, const ZipFlags &flags)
	{
		if (FileSystem::IsDirectory(filename))
		{
			std::string folderName = FileSystem::FileName(filename);
			std::vector<std::string> files = FileSystem::FilesInPath(filename);
			std::vector<std::string>::iterator it = files.begin();

			for (; it != files.end(); ++it)
			{
				std::ifstream input(it->c_str(), std::ios::binary);
				std::string nameInZip = it->substr(it->rfind(folderName + FileSystem::SEPARATOR), it->size());
				Add(input, nameInZip, flags);
				input.close();
			}
		}
		else
		{
			std::ifstream input(filename.c_str(), std::ios::binary);
			Add(input, FileSystem::FileName(filename), flags);
			input.close();
		}

		return true;
	}
}
