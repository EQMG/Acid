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
	/// A helper for C++ files.
	/// </summary>
	class ACID_EXPORT FileSystem
	{
	public:
		/// <summary>
		/// Gets if a file exists.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> If the file exists. </returns>
		static bool FileExists(const std::string &filepath);

		/// <summary>
		/// Deletes a file.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> If the file was deleted. </returns>
		static bool DeleteFile(const std::string &filepath);

		/// <summary>
		/// Creates a file, and the folder path.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="createFolders"> If folders should also be created. </param>
		/// <returns> If the file was created. </returns>
		static bool CreateFile(const std::string &filepath, const bool &createFolders = true);

		/// <summary>
		/// Clears the contents from a file.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> If the file was cleared. </returns>
		static bool ClearFile(const std::string &filepath);

		/// <summary>
		/// Creates a directory.
		/// </summary>
		/// <param name="path"> The directory to create. </param>
		/// <returns> If the folder was created. </returns>
		static bool CreateFolder(const std::string &path);

		/// <summary>
		/// Reads a text file into a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The string containing the read file. </returns>
		static std::optional<std::string> ReadTextFile(const std::string &filepath);

		/// <summary>
		/// Writes to a text file from a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The text data. </param>
		/// <returns> If the file was written to. </returns>
		static bool WriteTextFile(const std::string &filepath, const std::string &data);

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		///	<param name="filepath"> The filepath. </param>
		///	<param name="mode"> The read mode. </param>
		/// <returns> The char array loaded from the file. </returns>
		template<typename T>
		static std::optional<std::vector<T>> ReadBinaryFile(const std::string &filepath, const std::string &mode = "rb")
		{
			std::vector<T> data = {};

			const int32_t bufferSize = 1024;
			const bool useFile = filepath.c_str() && strcmp("-", filepath.c_str());

			if (FILE *fp = (useFile ? fopen(filepath.c_str(), mode.c_str()) : stdin))
			{
				T buf[bufferSize];

				while (size_t len = fread(buf, sizeof(T), bufferSize, fp))
				{
					data.insert(data.end(), buf, buf + len);
				}

				if (ftell(fp) == -1L)
				{
					if (ferror(fp))
					{
						Log::Error("Error reading file: '%s'\n", filepath.c_str());
						return {};
					}
				}
				else
				{
					if (sizeof(T) != 1 && (ftell(fp) % sizeof(T)))
					{
						Log::Error("Corrupted word found in file: '%s'\n", filepath.c_str());
						return {};
					}
				}

				if (useFile)
				{
					fclose(fp);
				}
			}
			else
			{
				Log::Error("File does not exist: '%s'\n", filepath.c_str());
				return {};
			}

			return data;
		}

		/// <summary>
		/// Writes to a binary file from a char vector.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The binary data. </param>
		/// <returns> If the file was written to. </returns>
		template<typename T>
		static bool WriteBinaryFile(const std::string &filepath, const std::vector<char> &data, const std::string &mode = "wb")
		{
			const bool useStdout = !filepath.c_str() || (filepath.c_str()[0] == '-' && filepath.c_str()[1] == '\0');

			if (FILE *fp = (useStdout ? stdout : fopen(filepath.c_str(), mode.c_str())))
			{
				size_t written = fwrite(data.data(), sizeof(T), data.size(), fp);

				if (data.size() != written)
				{
					Log::Error("Could not write to file: '%s'\n", filepath.c_str());
					return false;
				}

				if (!useStdout)
				{
					fclose(fp);
				}
			}
			else
			{
				Log::Error("File could not be opened: '%s'\n", filepath.c_str());
				return false;
			}

			return true;
		}

		/// <summary>
		/// Gets the current working directory.
		/// </summary>
		/// <returns> If the working directory. </returns>
		static std::string GetWorkingDirectory();

		/// <summary>
		/// Removes backslashes from directories.
		/// </summary>
		/// <returns> The path with only forward slashes. </returns>
		static std::string FixPaths(const std::string &filepath);

		/// <summary>
		/// Gets the file name from the full path.
		/// </summary>
		/// <returns> The file name. </returns>
		static std::string FindName(const std::string &filepath);

		/// <summary>
		/// Gets the file extension from the full path.
		/// </summary>
		/// <returns> The file extension. </returns>
		static std::string FindExt(const std::string &filepath);
	};
}
