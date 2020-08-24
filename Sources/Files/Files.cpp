#include "Files.hpp"

#include <iterator>
#include <physfs.h>
#include "Engine/Engine.hpp"
#include "Config.hpp"

namespace acid {
using std::streambuf;
using std::ios_base;

class FBuffer : public streambuf, NonCopyable {
public:
	explicit FBuffer(PHYSFS_File *file, std::size_t bufferSize = 2048) :
		bufferSize(bufferSize),
		file(file) {
		buffer = new char[bufferSize];
		auto end = buffer + bufferSize;
		setg(end, end, end);
		setp(buffer, end);
	}

	~FBuffer() {
		sync();
		delete[] buffer;
	}

private:
	int_type underflow() override {
		if (PHYSFS_eof(file)) {
			return traits_type::eof();
		}

		auto bytesRead = PHYSFS_readBytes(file, buffer, static_cast<PHYSFS_uint32>(bufferSize));
		if (bytesRead < 1)
			return traits_type::eof();

		setg(buffer, buffer, buffer + static_cast<size_t>(bytesRead));
		return static_cast<int_type>(*gptr());
	}

	pos_type seekoff(off_type pos, ios_base::seekdir dir, ios_base::openmode mode) override {
		switch (dir) {
		case std::ios_base::beg:
			PHYSFS_seek(file, pos);
			break;
		case std::ios_base::cur:
			// Subtract characters currently in buffer from seek position.
			PHYSFS_seek(file, (PHYSFS_tell(file) + pos) - (egptr() - gptr()));
			break;
		case std::ios_base::end:
			PHYSFS_seek(file, PHYSFS_fileLength(file) + pos);
			break;
		}

		if (mode & std::ios_base::in)
			setg(egptr(), egptr(), egptr());
		if (mode & std::ios_base::out)
			setp(buffer, buffer);

		return PHYSFS_tell(file);
	}

	pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override {
		PHYSFS_seek(file, pos);

		if (mode & std::ios_base::in)
			setg(egptr(), egptr(), egptr());
		if (mode & std::ios_base::out)
			setp(buffer, buffer);

		return PHYSFS_tell(file);
	}

	int_type overflow(int_type c = traits_type::eof()) override {
		if (pptr() == pbase() && c == traits_type::eof())
			return 0; // no-op

		if (PHYSFS_writeBytes(file, pbase(), static_cast<PHYSFS_uint32>(pptr() - pbase())) < 1)
			return traits_type::eof();

		if (c != traits_type::eof()) {
			if (PHYSFS_writeBytes(file, &c, 1) < 1)
				return traits_type::eof();
		}

		return 0;
	}

	int sync() override {
		return overflow();
	}

	char *buffer;
	size_t bufferSize;
protected:
	PHYSFS_File *file;
};

BaseFStream::BaseFStream(PHYSFS_File *file) :
	file(file) {
	if (file == NULL) {
		throw std::invalid_argument("Attempted to construct fstream with a NULL file");
	}
}

BaseFStream::~BaseFStream() {
	PHYSFS_close(file);
}

size_t BaseFStream::length() {
	return PHYSFS_fileLength(file);
}

PHYSFS_File *OpenWithMode(const std::filesystem::path &filename, FileMode openMode) {
	PHYSFS_File *file = nullptr;

	auto pathStr = filename.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');

	switch (openMode) {
	case FileMode::Write:
		file = PHYSFS_openWrite(pathStr.c_str());
		break;
	case FileMode::Append:
		file = PHYSFS_openAppend(pathStr.c_str());
		break;
	case FileMode::Read:
		file = PHYSFS_openRead(pathStr.c_str());
	}

	if (file == NULL)
		throw std::invalid_argument("File could not be found");
	return file;
}

IFStream::IFStream(const std::filesystem::path &filename) :
	BaseFStream(OpenWithMode(filename, FileMode::Read)),
	std::istream(new FBuffer(file)) {
}

IFStream::~IFStream() {
	delete rdbuf();
}

OFStream::OFStream(const std::filesystem::path &filename, FileMode writeMode) :
	BaseFStream(OpenWithMode(filename, writeMode)),
	std::ostream(new FBuffer(file)) {
}

OFStream::~OFStream() {
	delete rdbuf();
}

FStream::FStream(const std::filesystem::path &filename, FileMode openMode) :
	BaseFStream(OpenWithMode(filename, openMode)),
	std::iostream(new FBuffer(file)) {
}

FStream::~FStream() {
	delete rdbuf();
}

const bool Files::Registered = Register(Stage::Post);

Files::Files() {
	PHYSFS_init(Engine::Get()->GetArgv0().c_str());
	// TODO: Only when not installed. 
	if (std::filesystem::exists(ACID_RESOURCES_DEV))
		AddSearchPath(std::string(ACID_RESOURCES_DEV));
}

Files::~Files() {
	PHYSFS_deinit();
}

void Files::Update() {
}

void Files::AddSearchPath(const std::string &path) {
	if (std::find(searchPaths.begin(), searchPaths.end(), path) != searchPaths.end())
		return;

	if (PHYSFS_mount(path.c_str(), nullptr, true) == 0) {
		Log::Warning("Failed to mount path ", path, ", ", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()), '\n');
		return;
	}

	searchPaths.emplace_back(path);
}

void Files::RemoveSearchPath(const std::string &path) {
	auto it = std::find(searchPaths.begin(), searchPaths.end(), path);

	if (it == searchPaths.end())
		return;

	if (PHYSFS_unmount(path.c_str()) == 0) {
		Log::Warning("Failed to unmount path ", path, ", ", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()), '\n');
		return;
	}

	searchPaths.erase(it);
}

void Files::ClearSearchPath() {
	for (const auto &searchPath : searchPaths) {
		if (PHYSFS_unmount(searchPath.c_str()) == 0)
			Log::Warning("Failed to unmount path ", searchPath, ", ", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()), '\n');
	}

	searchPaths.clear();
}

bool Files::ExistsInPath(const std::filesystem::path &path) {
	if (PHYSFS_isInit() == 0) return false;

	auto pathStr = path.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	return PHYSFS_exists(pathStr.c_str()) != 0;
}

std::optional<std::string> Files::Read(const std::filesystem::path &path) {
	auto pathStr = path.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	auto fsFile = PHYSFS_openRead(pathStr.c_str());

	if (!fsFile) {
		if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
			Log::Error("Failed to open file ", path, ", ", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()), '\n');
			return std::nullopt;
		}

		std::ifstream is(path);
		std::stringstream buffer;
		buffer << is.rdbuf();
		return buffer.str();
	}

	auto size = PHYSFS_fileLength(fsFile);
	std::vector<uint8_t> data(size);
	PHYSFS_readBytes(fsFile, data.data(), static_cast<PHYSFS_uint64>(size));

	if (PHYSFS_close(fsFile) == 0)
		Log::Error("Failed to close file ", path, ", ", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()), '\n');

	return std::string(data.begin(), data.end());
}

std::vector<unsigned char> Files::ReadBytes(const std::filesystem::path &path) {
	IFStream file(path);
	file >> std::noskipws;

	auto fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> bytes;
	bytes.reserve(fileSize);

	std::copy(std::istream_iterator<unsigned char>(file),
		std::istream_iterator<unsigned char>(),
		back_inserter(bytes));

	return bytes;
}

std::vector<std::string> Files::FilesInPath(const std::filesystem::path &path, bool recursive) {
	auto pathStr = path.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	auto rc = PHYSFS_enumerateFiles(pathStr.c_str());

	std::vector<std::string> files;

	for (auto i = rc; *i; i++) {
		/*if (IsDirectory(*i)) {
			if (recursive) {
				auto filesInFound = FilesInPath(*i, recursive);
				files.insert(result.end(), filesInFound.begin(), filesInFound.end());
			}
		} else {*/
		files.emplace_back(*i);
		//}
	}

	PHYSFS_freeList(rc);
	return files;
}

std::istream &Files::SafeGetLine(std::istream &is, std::string &t) {
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	auto sb = is.rdbuf();

	if (se) {
		while (true) {
			auto c = sb->sbumpc();

			switch (c) {
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
					is.setstate(std::ios::eofbit);

				return is;
			default:
				t += static_cast<char>(c);
			}
		}
	}

	return is;
}
}
