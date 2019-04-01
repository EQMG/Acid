#pragma once

#include "Engine/Log.hpp"

namespace acid
{
/**
 * A group of functions used for creation, loading, writing, and more.
 */
class ACID_EXPORT FileSystem
{
public:
	static const char Separator;
	static const char AltSeparator;

	/**
	 * Gets the current working directory.
	 * @return If the working directory.
	 */
	static std::string GetWorkingDirectory();

	/**
	 * Gets if a file exists.
	 * @param path The path.
	 * @return If the file exists.
	 */
	static bool Exists(const std::string &path);

	/**
	 * Gets if a path is a file exists.
	 * @param path The path.
	 * @return If the path is a file.
	 */
	static bool IsFile(const std::string &path);

	/**
	 * Gets if a path is a directory exists.
	 * @param path The path.
	 * @return If the path is a directory.
	 */
	static bool IsDirectory(const std::string &path);

	/**
	 * Gets if a path is readable.
	 * @param path The path.
	 * @return If the path is readable.
	 */
	static bool IsReadable(const std::string &path);

	/**
	 * Gets if a path is writeable.
	 * @param path The path.
	 * @return If the path is writeable.
	 */
	static bool IsWriteable(const std::string &path);

	/**
	 * Gets the last time a file was modified.
	 * @param path The path.
	 * @return The last date a file was modified, if it exists.
	 */
	static long LastModified(const std::string &path);

	/**
	 * Sets the time of modification of a file to now.
	 * @param path The path.
	 */
	static void Touch(const std::string &path);

	/**
	 * Finds all the files in a path.
	 * @param path The path to search.
	 * @param recursive If paths will be recursively searched.
	 * @return The files found.
	 */
	static std::vector<std::string> FilesInPath(const std::string &path, const bool &recursive = true);

	/**
	 * Creates a path, will create folders and files.
	 * @param path The path.
	 * @return If the path was created.
	 */
	static bool Create(const std::string &path);

	/**
	 * Deletes the last element in a path (file or directory).
	 * @param path The path.
	 * @return If the path was deleted.
	 */
	static bool Delete(const std::string &path);

	/**
	 * Reads a text file into a string.
	 * @param filename The filename.
	 * @return The string containing the read file.
	 */
	static std::optional<std::string> ReadTextFile(const std::string &filename);

	/**
	 * Writes to a text file from a string.
	 * @param filename The filename.
	 * @param data The text data.
	 * @return If the file was written to.
	 */
	static bool WriteTextFile(const std::string &filename, const std::string &data);

	/**
	 * Reads a binary file into a char array.
	 * @param filename The filename.
	 * @param mode The read mode.
	 * @return The char array loaded from the file.
	 */
	static std::optional<std::vector<char>> ReadBinaryFile(const std::string &filename, const std::string &mode = "rb");

	/**
	 * Writes to a binary file from a char vector.
	 * @param filename The filename.
	 * @param data The binary data.
	 * @param mode The write mode.
	 * @return If the file was written to.
	 */
	static bool WriteBinaryFile(const std::string &filename, const std::vector<char> &data, const std::string &mode = "wb");

	/**
	 * Clears the contents from a file.
	 * @param filename The filename.
	 * @return If the file was cleared.
	 */
	static bool ClearFile(const std::string &filename);

	/**
	 * Gets the parent directory of a path.
	 * @param path The path to get the parent directory of.
	 * @return The parent directory.
	 */
	static std::string ParentDirectory(const std::string &path);

	/**
	 * Gets the file name from a path.
	 * @param path The path to get the name of.
	 * @return The file name.
	 */
	static std::string FileName(const std::string &path);

	/**
	 * Gets the file suffix (extension) from a path.
	 * @param path The path to get the suffix of.
	 * @return The file suffix.
	 */
	static std::string FileSuffix(const std::string &path);

	/**
	 * Joins parts of a path together.
	 * @param parts The parts to join together.
	 * @return The joined path.
	 */
	static std::string JoinPath(const std::vector<std::string> &parts);

	/**
	 * Splits a path by a system separator, will also use the alt separator.
	 * @param path The path to split.
	 * @param delim The separator to split by.
	 * @return The split path.
	 */
	static std::vector<std::string> SplitPath(const std::string &path, const char &delim = Separator);
};
}
