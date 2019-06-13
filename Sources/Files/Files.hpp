#pragma once

#include "Engine/Engine.hpp"

struct PHYSFS_File;

namespace acid
{
enum class FileMode
{
	Read, Write, Append
};

class ACID_EXPORT BaseFStream
{
protected:
	PHYSFS_File *file;
public:
	explicit BaseFStream(PHYSFS_File *file);

	virtual ~BaseFStream();

	size_t length();
};

class ACID_EXPORT IFStream :
	public BaseFStream,
	public std::istream
{
public:
	explicit IFStream(const std::filesystem::path &filename);

	virtual ~IFStream();
};

class ACID_EXPORT OFStream :
	public BaseFStream,
	public std::ostream
{
public:
	explicit OFStream(const std::filesystem::path &filename, const FileMode &writeMode = FileMode::Write);

	virtual ~OFStream();
};

class ACID_EXPORT FStream :
	public BaseFStream,
	public std::iostream
{
public:
	explicit FStream(const std::filesystem::path &filename, const FileMode &openMode = FileMode::Read);

	virtual ~FStream();
};

/**
 * @brief Module used for managing files on engine updates.
 */
class ACID_EXPORT Files :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Files *Get() { return Engine::Get()->GetModule<Files>(); }

	Files();

	~Files();

	void Update() override;

	/**
	 * Adds an file search path.
	 * @param path The path to add.
	 */
	void AddSearchPath(const std::string &path);

	/**
	 * Removes a file search path.
	 * @param path The path to remove.
	 */
	void RemoveSearchPath(const std::string &path);

	/**
	 * Clears all file search paths.
	 */
	void ClearSearchPath();

	/**
	 * Gets if the path is found in one of the search paths.
	 * @param path The path to look for.
	 * @return If the path is found in one of the searches.
	 */
	static bool ExistsInPath(const std::filesystem::path &path);

	/**
	 * Reads a file found by real or partial path.
	 * @param path The path to read.
	 * @return The data read from the file.
	 */
	static std::optional<std::string> Read(const std::filesystem::path &path);

	/**
	 * Finds all the files in a path.
	 * @param path The path to search.
	 * @param recursive If paths will be recursively searched.
	 * @return The files found.
	 */
	static std::vector<std::string> FilesInPath(const std::filesystem::path &path, const bool &recursive = true);

	/**
	 * Gets the next line from a stream.
	 * @param is The input stream.
	 * @param t The next string.
	 * @return The input stream.
	 */
	static std::istream &SafeGetLine(std::istream &is, std::string &t);

private:
	std::vector<std::string> m_searchPaths;
};
}
