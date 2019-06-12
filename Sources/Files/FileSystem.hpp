#pragma once

#include "Engine/Log.hpp"

namespace acid
{
/**
 * @brief A group of functions used for creation, loading, writing, and more.
 */
class ACID_EXPORT FileSystem
{
public:
	static const char Separator;
	static const char AltSeparator;

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
