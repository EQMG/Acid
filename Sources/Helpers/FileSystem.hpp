#pragma once

#include <vector>
#include <cstring>
#include <string>
#include "Engine/Exports.hpp"

namespace fl
{
	/// <summary>
	/// A helper for C++ files.
	/// </summary>
	class FL_EXPORT FileSystem
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
		static void DeleteFile(const std::string &filepath);

		/// <summary>
		/// Creates a file, and the folder path.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="createFolders"> If folders should also be created. </param>
		static void CreateFile(const std::string &filepath, const bool &createFolders = true);

		/// <summary>
		/// Clears the contents from a file.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		static void ClearFile(const std::string &filepath);

		/// <summary>
		/// Creates a directory.
		/// </summary>
		/// <param name="path"> The directory to create. </param>
		static void CreateFolder(const std::string &path);

		/// <summary>
		/// Reads a text file into a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <returns> The string containing the read file. </returns>
		static std::string ReadTextFile(const std::string &filepath);

		/// <summary>
		/// Writes to a text file from a string.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The text data. </param>
		static void WriteTextFile(const std::string &filepath, const std::string &data);

		/// <summary>
		/// Reads a binary file into a char array.
		/// </summary>
		///	<param name="filepath"> The filepath. </param>
		///	<param name="mode"> The read mode. </param>
		/// <returns> The char array loaded from the file. </returns>
		template<typename T>
		static std::vector<T> ReadBinaryFile(const std::string &filepath, const std::string &mode = "rb")
		{
			std::vector<T> data = {};

			const int bufferSize = 1024;
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
						fprintf(stderr, "Error reading file: '%s'\n", filepath.c_str());
						return data;
					}
				}
				else
				{
					if (sizeof(T) != 1 && (ftell(fp) % sizeof(T)))
					{
						fprintf(stderr, "Corrupted word found in file: '%s'\n", filepath.c_str());
						return data;
					}
				}

				if (useFile)
				{
					fclose(fp);
				}
			}
			else
			{
				fprintf(stderr, "File does not exist: '%s'\n", filepath.c_str());
			}

			return data;
		}

		/// <summary>
		/// Writes to a binary file from a char vector.
		/// </summary>
		/// <param name="filepath"> The filepath. </param>
		/// <param name="data"> The binary data. </param>
		template<typename T>
		static void WriteBinaryFile(const std::string &filepath, const std::vector<char> &data, const std::string &mode = "wb")
		{
			const bool useStdout = !filepath.c_str() || (filepath.c_str()[0] == '-' && filepath.c_str()[1] == '\0');

			if (FILE *fp = (useStdout ? stdout : fopen(filepath.c_str(), mode.c_str())))
			{
				size_t written = fwrite(data.data(), sizeof(T), data.size(), fp);

				if (data.size() != written)
				{
					fprintf(stderr, "Could not write to file: '%s'\n", filepath.c_str());
					return;
				}

				if (!useStdout)
				{
					fclose(fp);
				}
			}
			else
			{
				fprintf(stderr, "File could not be opened: '%s'\n", filepath.c_str());
				return;
			}
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
