#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace acid
{
	enum ZipFlags
	{
		ZIP_OVERWITE = 0x01,  // -o
		ZIP_APPEND = 0x02, // -a
		ZIP_STORE = 0x04, // -0
		ZIP_FASTER = 0x08, // -1
		ZIP_BETTER = 0x10, // -9
		ZIP_NOPATHS = 0x20 // -j
	};

	typedef void *zipFile;
	typedef void *ourmemory_t;
	typedef void *zlib_filefunc_def;

	class Zipper
	{
	private:
		std::string m_filename;
		std::string m_password;

		zipFile m_zf;
		ourmemory_t m_zipmem;
		zlib_filefunc_def m_filefunc;
	public:
		explicit Zipper(const std::string &filename, const std::string &password = "");

		void Open();

		void Close();

		bool Add(std::istream &inputStream, const std::string &nameInZip = "", const ZipFlags &flags = ZIP_BETTER);

		bool Add(const std::string &filename, const ZipFlags &flags = ZIP_BETTER);
	};
}
