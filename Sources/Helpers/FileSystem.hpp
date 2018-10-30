#pragma once

#include <cstring>
#include <optional>
#include <string>
#include <vector>
#include "Engine/Exports.hpp"
#include "Engine/Log.hpp"

namespace acid
{
	/// <summary>
	/// A helper for file creation, loading, writing, and more.
	/// </summary>
	class ACID_EXPORT FileSystem
	{
	public:
		static const char SEPARATOR;
		static const char ALT_SEPARATOR;

		/// <summary>
		/// Gets the current working directory.
		/// </summary>
		/// <returns> If the working directory. </returns>
		static std::string GetWorkingDirectory();

		/// <summary>
		/// Gets if a file exists.
		/// </summary>
		/// <param name="filename"> The filename. </param>
		/// <returns> If the file exists. </returns>
		static bool Exists(const std::string &path);

		/// <summary>
		/// Gets if a path is a file exists.
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path is a file. </returns>
		static bool IsFile(const std::string &path);

		/// <summary>
		/// Gets if a path is a directory exists.
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path is a directory. </returns>
		static bool IsDirectory(const std::string &path);

		/// <summary>
		/// Gets if a path is readable.
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path is readable. </returns>
		static bool IsReadable(const std::string &path);

		/// <summary>
		/// Gets if a path is writeable.
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path is writeable. </returns>
		static bool IsWriteable(const std::string &path);

		/// <summary>
		/// Finds all the files in a path.
		/// </summary>
		/// <param name="path"> The path to search. </param>
		/// <param name="recursive"> If paths will be recursively searched. </param>
		/// <returns> The files found. </returns>
		static std::vector<std::string> FilesInPath(const std::string &path, const bool &recursive = true);

		/// <summary>
		/// Creates a path, will create folders and files.
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path was created. </returns>
		static bool Create(const std::string &path);

		/// <summary>
		/// Deletes the last element in a path (file or directory).
		/// </summary>
		/// <param name="path"> The path. </param>
		/// <returns> If the path was deleted. </returns>
		static bool Delete(const std::string &path);

		/// <summary>
		/// Reads a text file into a string.
		/// </summary>
		/// <param name="filename"> The filename. </param>
		/// <returns> The string containing the read file. </returns>
		static std::optional<std::string> ReadTextFile(const std::string &filename);

		/// <summary>
		/// Writes to a text file from a string.
		/// </summary>
		/// <param name="filename"> The filename. </param>
		/// <param name="data"> The text data. </param>
		/// <returns> If the file was written to. </returns>
		static bool WriteTextFile(const std::string &filename, const std::string &data);

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		///	<param name="filename"> The filename. </param>
		///	<param name="mode"> The read mode. </param>
		/// <returns> The char array loaded from the file. </returns>
		static std::optional<std::vector<char>> ReadBinaryFile(const std::string &filename, const std::string &mode = "rb");

		/// <summary>
		/// Writes to a binary file from a char vector.
		/// </summary>
		/// <param name="filename"> The filename. </param>
		/// <param name="data"> The binary data. </param>
		/// <returns> If the file was written to. </returns>
		static bool WriteBinaryFile(const std::string &filename, const std::vector<char> &data, const std::string &mode = "wb");

		/// <summary>
		/// Clears the contents from a file.
		/// </summary>
		/// <param name="filename"> The filename. </param>
		/// <returns> If the file was cleared. </returns>
		static bool ClearFile(const std::string &filename);

		/// <summary>
		/// Gets the parent directory of a path.
		/// </summary>
		/// <param name="path"> The path to get the parent directory of. </param>
		/// <returns> The parent directory. </returns>
		static std::string ParentDirectory(const std::string &path);

		/// <summary>
		/// Gets the file name from a path.
		/// </summary>
		/// <param name="path"> The path to get the name of. </param>
		/// <returns> The file name. </returns>
		static std::string FileName(const std::string &path);

		/// <summary>
		/// Gets the file suffix (extension) from a path.
		/// </summary>
		/// <param name="path"> The path to get the suffix of. </param>
		/// <returns> The file suffix. </returns>
		static std::string FileSuffix(const std::string &path);

		/// <summary>
		/// Joins parts of a path together.
		/// </summary>
		/// <param name="parts"> The parts to join together. </param>
		/// <returns> The joined path. </returns>
		static std::string JoinPath(const std::vector<std::string> &parts);

		/// <summary>
		/// Splits a path by a system separator, will also use the alt separator.
		/// </summary>
		/// <param name="path"> The path to split. </param>
		/// <param name="delim"> The separator to split by. </param>
		/// <returns> The split path. </returns>
		static std::vector<std::string> SplitPath(const std::string &path, const char &delim = SEPARATOR);
	};
}
