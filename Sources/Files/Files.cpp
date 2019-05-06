#include "Files.hpp"

#include <physfs.h>
#include "Engine/Engine.hpp"
#include "FileSystem.hpp"

namespace acid
{
using std::streambuf;
using std::ios_base;

class FBuffer :
	public NonCopyable,
	public streambuf
{
public:
	explicit FBuffer(PHYSFS_File *file, const std::size_t &bufferSize = 2048) :
		m_bufferSize(bufferSize),
		m_file(file)
	{
		m_buffer = new char[m_bufferSize];
		auto end = m_buffer + m_bufferSize;
		setg(end, end, end);
		setp(m_buffer, end);
	}

	~FBuffer()
	{
		sync();
		delete[] m_buffer;
	}

private:
	int_type underflow() override
	{
		if (PHYSFS_eof(m_file))
		{
			return traits_type::eof();
		}

		auto bytesRead = PHYSFS_readBytes(m_file, m_buffer, static_cast<PHYSFS_uint32>(m_bufferSize));

		if (bytesRead < 1)
		{
			return traits_type::eof();
		}

		setg(m_buffer, m_buffer, m_buffer + static_cast<size_t>(bytesRead));
		return static_cast<int_type>(*gptr());
	}

	pos_type seekoff(off_type pos, ios_base::seekdir dir, ios_base::openmode mode) override
	{
		switch (dir)
		{
		case std::ios_base::beg:
			PHYSFS_seek(m_file, pos);
			break;
		case std::ios_base::cur:
			// Subtract characters currently in buffer from seek position.
			PHYSFS_seek(m_file, (PHYSFS_tell(m_file) + pos) - (egptr() - gptr()));
			break;
		case std::ios_base::end:
			PHYSFS_seek(m_file, PHYSFS_fileLength(m_file) + pos);
			break;
		}

		if (mode & std::ios_base::in)
		{
			setg(egptr(), egptr(), egptr());
		}

		if (mode & std::ios_base::out)
		{
			setp(m_buffer, m_buffer);
		}

		return PHYSFS_tell(m_file);
	}

	pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override
	{
		PHYSFS_seek(m_file, pos);

		if (mode & std::ios_base::in)
		{
			setg(egptr(), egptr(), egptr());
		}

		if (mode & std::ios_base::out)
		{
			setp(m_buffer, m_buffer);
		}

		return PHYSFS_tell(m_file);
	}

	int_type overflow(int_type c = traits_type::eof()) override
	{
		if (pptr() == pbase() && c == traits_type::eof())
		{
			return 0; // no-op
		}

		if (PHYSFS_writeBytes(m_file, pbase(), static_cast<PHYSFS_uint32>(pptr() - pbase())) < 1)
		{
			return traits_type::eof();
		}

		if (c != traits_type::eof())
		{
			if (PHYSFS_writeBytes(m_file, &c, 1) < 1)
			{
				return traits_type::eof();
			}
		}

		return 0;
	}

	int sync() override
	{
		return overflow();
	}

	char *m_buffer;
	size_t m_bufferSize;
protected:
	PHYSFS_File *m_file;
};

BaseFStream::BaseFStream(PHYSFS_File *file) :
	file(file)
{
	if (file == NULL)
	{
		throw std::invalid_argument("Attempted to construct fstream with a NULL file");
	}
}

BaseFStream::~BaseFStream()
{
	PHYSFS_close(file);
}

size_t BaseFStream::length()
{
	return PHYSFS_fileLength(file);
}

PHYSFS_File *OpenWithMode(char const *filename, FileMode openMode)
{
	PHYSFS_File *file = NULL;

	switch (openMode)
	{
	case FileMode::Write:
		file = PHYSFS_openWrite(filename);
		break;
	case FileMode::Append:
		file = PHYSFS_openAppend(filename);
		break;
	case FileMode::Read:
		file = PHYSFS_openRead(filename);
	}

	if (file == NULL)
	{
		throw std::invalid_argument("File not found: " + std::string(filename));
	}

	return file;
}

IFStream::IFStream(const std::string &filename) :
	BaseFStream(OpenWithMode(filename.c_str(), FileMode::Read)),
	std::istream(new FBuffer(file))
{
}

IFStream::~IFStream()
{
	delete rdbuf();
}

OFStream::OFStream(const std::string &filename, const FileMode &writeMode) :
	BaseFStream(OpenWithMode(filename.c_str(), writeMode)),
	std::ostream(new FBuffer(file))
{
}

OFStream::~OFStream()
{
	delete rdbuf();
}

FStream::FStream(const std::string &filename, const FileMode &openMode) :
	BaseFStream(OpenWithMode(filename.c_str(), openMode)),
	std::iostream(new FBuffer(file))
{
}

FStream::~FStream()
{
	delete rdbuf();
}

Files::Files()
{
	PHYSFS_init(Engine::Get()->GetArgv0().c_str());
}

Files::~Files()
{
	PHYSFS_deinit();
}

void Files::Update()
{
}

void Files::AddSearchPath(const std::string &path)
{
	if (std::find(m_searchPaths.begin(), m_searchPaths.end(), path) != m_searchPaths.end())
	{
		return;
	}

	if (PHYSFS_mount(path.c_str(), nullptr, true) == 0)
	{
		Log::Error("File System error while adding a path or zip(%s): %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return;
	}

	m_searchPaths.emplace_back(path);
}

void Files::RemoveSearchPath(const std::string &path)
{
	auto it = std::find(m_searchPaths.begin(), m_searchPaths.end(), path);

	if (it == m_searchPaths.end())
	{
		return;
	}

	if (PHYSFS_unmount(path.c_str()) == 0)
	{
		Log::Error("File System error while removing a path: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return;
	}

	m_searchPaths.erase(it);
}

void Files::ClearSearchPath()
{
	for (const auto &path : std::vector<std::string>(m_searchPaths))
	{
		RemoveSearchPath(path);
	}
}

bool Files::ExistsInPath(const std::string &path)
{
	if (PHYSFS_isInit() == 0)
	{
		return false;
	}

	return PHYSFS_exists(path.c_str()) != 0;
}

std::optional<std::string> Files::Read(const std::string &path)
{
	auto fsFile = PHYSFS_openRead(path.c_str());

	if (fsFile == nullptr)
	{
		if (!FileSystem::Exists(path) || !FileSystem::IsFile(path))
		{
			Log::Error("Error while opening file to load %s: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return {};
		}

		return FileSystem::ReadTextFile(path);
	}

	auto size = PHYSFS_fileLength(fsFile);
	std::vector<uint8_t> data(size);
	PHYSFS_readBytes(fsFile, data.data(), static_cast<PHYSFS_uint64>(size));

	if (PHYSFS_close(fsFile) == 0)
	{
		Log::Error("Error while closing file %s: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return std::string(data.begin(), data.end());
}

std::vector<std::string> Files::FilesInPath(const std::string &path, const bool &recursive)
{
	std::vector<std::string> files;
	auto rc = PHYSFS_enumerateFiles(path.c_str());
	char **i;

	for (i = rc; *i != nullptr; i++)
	{
		/*if (IsDirectory(*i))
		{
			if (recursive)
			{
				auto filesInFound = FilesInPath(*i, recursive);
				files.insert(result.end(), filesInFound.begin(), filesInFound.end());
			}
		}
		else
		{*/
		files.emplace_back(*i);
		//}
	}

	PHYSFS_freeList(rc);
	return files;
}

std::istream &Files::SafeGetLine(std::istream &is, std::string &t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf *sb = is.rdbuf();

	if (se)
	{
		for (;;)
		{
			int c = sb->sbumpc();

			switch (c)
			{
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
				{
					sb->sbumpc();
				}
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
				{
					is.setstate(std::ios::eofbit);
				}

				return is;
			default:
				t += static_cast<char>(c);
			}
		}
	}

	return is;
}
}
