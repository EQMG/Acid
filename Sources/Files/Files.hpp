#pragma once

#include "Engine/Engine.hpp"

struct PHYSFS_File;

namespace acid
{
enum class FileMode
{
	Read, Write, Append
};

class BaseFStream
{
protected:
	PHYSFS_File *file;
public:
	explicit BaseFStream(PHYSFS_File *file);

	virtual ~BaseFStream();

	size_t length();
};

class IFStream :
	public BaseFStream,
	public std::istream
{
public:
	explicit IFStream(const std::string &filename);

	virtual ~IFStream();
};

class OFStream :
	public BaseFStream,
	public std::ostream
{
public:
	explicit OFStream(const std::string &filename, const FileMode &writeMode = FileMode::Write);

	virtual ~OFStream();
};

class FStream :
	public BaseFStream,
	public std::iostream
{
public:
	explicit FStream(const std::string &filename, const FileMode &openMode = FileMode::Read);

	virtual ~FStream();
};

/// <summary>
/// A module used for managing files on engine updates.
/// </summary>
class ACID_EXPORT Files :
	public Module
{
public:
	/// <summary>
	/// Gets this engine instance.
	/// </summary>
	/// <returns> The current module instance. </returns>
	static Files *Get() { return Engine::Get()->GetModuleManager().Get<Files>(); }

	Files();

	~Files();

	void Update() override;

	/// <summary>
	/// Adds an file search path, ensure <seealso cref="#SetBaseDirectory()"/> is called once before.
	/// </summary>
	/// <param name="path"> The path to add. </param>
	void AddSearchPath(const std::string &path);

	/// <summary>
	/// Removes a file search path.
	/// </summary>
	/// <param name="path"> The path to remove. </param>
	void RemoveSearchPath(const std::string &path);

	/// <summary>
	/// Clears all file search paths.
	/// </summary>
	void ClearSearchPath();

	/// <summary>
	/// Gets if the path is found in one of the search paths.
	/// </summary>
	/// <param name="path"> The path to look for. </param>
	/// <returns> If the path is found in one of the searches. </returns>
	static bool ExistsInPath(const std::string &path);

	/// <summary>
	/// Reads a file found by real or partial path.
	/// </summary>
	/// <param name="path"> The path to read. </param>
	/// <returns> The data read from the file. </returns>
	static std::optional<std::string> Read(const std::string &path);

	/// <summary>
	/// Finds all the files in a path.
	/// </summary>
	/// <param name="path"> The path to search. </param>
	/// <param name="recursive"> If paths will be recursively searched. </param>
	/// <returns> The files found. </returns>
	static std::vector<std::string> FilesInPath(const std::string &path, const bool &recursive = true);

	// http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
	static std::istream &SafeGetLine(std::istream &is, std::string &t);

private:
	std::vector<std::string> m_searchPaths;
};
}
