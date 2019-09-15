#pragma once

/**
 * https://github.com/troldal/Zippy
 *
 * MIT License
 *
 * Copyright (c) 2019 Kenneth Troldal Balslev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ZipEntry.h"

namespace acid {
/**
 * @brief The ZipArchive class represents the zip archive file as a whole. It consists of the individual zip entries, which
 * can be both files and folders. It is the main access point into a .zip archive on disk and can be
 * used to create new archives and to open and modify existing archives.
 * @details
 * #### Implementation and usage details
 * Using the ZipArchive class, it is possible to create new .zip archive files, as well as open and modify existing ones.
 *
 * A ZipArchive object holds a mz_zip_archive object (a miniz struct representing a .zip archive) as well as a std::vector
 * with ZipEntry objects representing each file (entry) in the archive. The actual entry data is lazy-instantiated, so that
 * the data is only loaded from the .zip archive when it is actually needed.
 *
 * Note that the actual files in the .zip archive can be retrieved via the ZipEntry interface, not the ZipArchive interface.
 */
class ZipArchive : public NonCopyable {
public:
	/**
	 * @brief Constructor. Constructs a null-archive, which can be used for creating a new .zip file
	 * or opening an existing one.
	 * @warning Before using an archive object that has been default constructed, a call to either Open() or Create() must be
	 * performed. Otherwise, the object will be in a null-state and calls to member functions will be undefined.
	 */
	explicit ZipArchive() = default;

	/**
	 * @brief Constructor. Constructs an archive object, using the filename input parameter. If the file already exists,
	 * it will be opened. Otherwise, a new object will be created.
	 * @details
	 * #### Implementation details
	 * The constructors tries to open an std::ifstream. If it is valid, it means that a file already exists
	 * and will be opened. Otherwise, the file does not exist and will be created.
	 * @param filename The name of the file to open or create.
	 */
	explicit ZipArchive(const std::filesystem::path &filename);

	/**
	 * @brief Destructor.
	 * @note The destructor will call the Close() member function. If the archive has been modified but not saved,
	 * all changes will be discarded.
	 */
	virtual ~ZipArchive();

	/**
	 * @brief Creates a new (empty) archive file with the given filename.
	 * @details
	 * #### Implementation details
	 * A new archive file is created and initialized and thereafter closed, creating an empty archive file.
	 * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the Open() member function.
	 * @param filename The filename for the new archive.
	 */
	void Create(const std::filesystem::path &filename);

	/**
	 * @brief Open an existing archive file with the given filename.
	 * @details
	 * ##### Implementation details
	 * The archive file is opened and meta data for all the entries in the archive is loaded into memory.
	 * @param filename The filename of the archive to open.
	 * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
	 * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
	 */
	void Open(const std::filesystem::path &filename);

	/**
	 * @brief Close the archive for reading and writing.
	 * @note If the archive has been modified but not saved, all changes will be discarded.
	 */
	void Close();

	/**
	 * @brief Checks if the archive file is open for reading and writing.
	 * @return true if it is open; otherwise false;
	 */
	bool IsOpen() const { return m_isOpen; }

	/**
	 * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
	 * directories, files or both.
	 * @param includeDirs If true, the list will include directories; otherwise not. Default is true
	 * @param includeFiles If true, the list will include files; otherwise not. Default is true
	 * @return A std::vector of std::strings with the entry names.
	 */
	std::vector<std::string> GetEntryNames(bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Get a list of the entries in a specific directory of the archive. Depending on the input parameters,
	 * the list will include directories, files or both.
	 * @param dir The directory with the entries to get.
	 * @param includeDirs If true, the list will include directories; otherwise not. Default is true
	 * @param includeFiles If true, the list will include files; otherwise not. Default is true
	 * @return A std::vector of std::strings with the entry names. The directory itself is not included.
	 */
	std::vector<std::string> GetEntryNamesInDir(const std::string &dir, bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Get a list of the metadata for entries in the archive. Depending on the input parameters, the list will include
	 * directories, files or both.
	 * @param includeDirs If true, the list will include directories; otherwise not. Default is true
	 * @param includeFiles If true, the list will include files; otherwise not. Default is true
	 * @return A std::vector of ZipEntryMetaData structs with the entry metadata.
	 */
	std::vector<ZipEntryMetaData> GetMetaData(bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Get a list of the metadata for entries in a specific directory of the archive. Depending on the input
	 * parameters, the list will include directories, files or both.
	 * @param dir The directory with the entries to get.
	 * @param includeDirs If true, the list will include directories; otherwise not. Default is true
	 * @param includeFiles If true, the list will include files; otherwise not. Default is true
	 * @return A std::vector of ZipEntryMetaData structs with the entry metadata. The directory itself is not included.
	 */
	std::vector<ZipEntryMetaData> GetMetaDataInDir(const std::string &dir, bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Get the number of entries in the archive. Depending on the input parameters, the number will include
	 * directories, files or both.
	 * @param includeDirs If true, the number will include directories; otherwise not. Default is true
	 * @param includeFiles If true, the number will include files; otherwise not. Default is true
	 * @return An int with the number of entries.
	 */
	int GetNumEntries(bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Get the number of entries in a specific directory of the archive. Depending on the input parameters,
	 * the list will include directories, files or both.
	 * @param dir The directory with the entries to get.
	 * @param includeDirs If true, the number will include directories; otherwise not. Default is true.
	 * @param includeFiles If true, the number will include files; otherwise not. Default is true.
	 * @return An int with the number of entries. The directory itself is not included.
	 */
	int GetNumEntriesInDir(const std::string &dir, bool includeDirs = true, bool includeFiles = true) const;

	/**
	 * @brief Check if an entry with a given name exists in the archive.
	 * @param entryName The name of the entry to check for.
	 * @return true if it exists; otherwise false;
	 */
	bool HasEntry(const std::string &entryName) const;

	/**
	 * @brief Save the archive with a new name. The original archive will remain unchanged.
	 * @param filename The new filename.
	 * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
	 */
	void Write(std::filesystem::path filename = "");

	/**
	 * @brief
	 * @param stream
	 */
	void Write(std::ostream &stream);

	/**
	 * @brief Deletes an entry from the archive.
	 * @param name The name of the entry to delete.
	 */
	void DeleteEntry(const std::string &name);

	/**
	 * @brief Get the entry with the specified name.
	 * @param name The name of the entry in the archive.
	 * @return A ZipEntry object with the requested entry.
	 */
	ZipEntry *GetEntry(const std::string &name);

	/**
	 * @brief Extract the entry with the provided name to the destination path.
	 * @param name The name of the entry to extract.
	 * @param dest The path to extract the entry to.
	 */
	void ExtractEntry(const std::string &name, const std::filesystem::path &dest);

	/**
	 * @brief Extract all entries in the given directory to the destination path.
	 * @param dir The name of the directory to extract.
	 * @param dest The path to extract the entry to.
	 */
	void ExtractDir(const std::string &dir, const std::filesystem::path &dest);

	/**
	 * @brief Extract all archive contents to the destination path.
	 * @param dest The path to extract the entry to.
	 */
	void ExtractAll(const std::filesystem::path &dest);

	/**
	 * @brief Add a new entry to the archive.
	 * @param name The name of the entry to add.
	 * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
	 * @return The ZipEntry object that has been added to the archive.
	 * @note If an entry with given name already exists, it will be overwritten.
	 */
	ZipEntry *AddEntry(const std::string &name, const ZipEntryData &data);

	/**
	 * @brief Add a new entry to the archive.
	 * @param name The name of the entry to add.
	 * @param data The std::string data to add.
	 * @return The ZipEntry object that has been added to the archive.
	 * @note If an entry with given name already exists, it will be overwritten.
	 */
	ZipEntry *AddEntry(const std::string &name, const std::string &data);

	/**
	 * @brief Add a new entry to the archive, using another entry as input. This is mostly used for cloning existing entries
	 * in the same archive, or for copying entries from one archive to another.
	 * @param name The name of the entry to add.
	 * @param entry The ZipEntry to add.
	 * @return The ZipEntry object that has been added to the archive.
	 * @note If an entry with given name already exists, it will be overwritten.
	 */
	ZipEntry *AddEntry(const std::string &name, const ZipEntry &entry);

private:
	/**
	 * @brief Throws the correct exception based on the error code, and adds a description.
	 * @param error The miniz errorcode
	 * @param errorString A description of the error.
	 */
	static void ThrowException(mz_zip_error error, const std::string &errorString);

	/**
	 * @brief Generates a random filename, which is used to generate a temporary archive when modifying and saving
	 * archive files.
	 * @param length The length of the filename to create.
	 * @return Returns the generated filename, appended with '.tmp'.
	 */
	static std::string GenerateRandomName(int length);

	mz_zip_archive m_archive = {}; /// The struct used by miniz, to handle archive files.
	std::filesystem::path m_archivePath; /// The path of the archive file.
	bool m_isOpen = false; /// A flag indicating if the file is currently open for reading and writing.

	std::vector<std::unique_ptr<ZipEntry>> m_zipEntries; /// Data structure for all entries in the archive.
};
}
