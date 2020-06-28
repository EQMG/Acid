/*
    MIT License

    Copyright (c) 2019 Kenneth Troldal Balslev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

#ifndef Zippy_ZIPARCHIVE_HPP
#define Zippy_ZIPARCHIVE_HPP

// ===== Standard Library Includes
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <memory>
#include <fstream>

// ===== Other Includes
#ifdef _WIN32
#include <direct.h>
#endif
#include <miniz/miniz.h>
#include "ZipException.hpp"
#include "ZipEntry.hpp"

namespace Zippy
{

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
     * The following example shows how a new .zip archive can be created and new entries added.
     * ```cpp
     * int main() {
     *
     *       // ===== Creating empty archive
     *       Zippy::ZipArchive arch;
     *       arch.Create("./TestArchive.zip");
     *
     *       // ===== Adding 10 entries to the archive
     *       for (int i = 0; i <= 9; ++i)
     *           arch.AddEntry(to_string(i) + ".txt", "this is test " + to_string(i));
     *
     *       // ===== Delete the first entry
     *       arch.DeleteEntry("0.txt");
     *
     *       // ===== Save and close the archive
     *       arch.Save();
     *       arch.Close();
     *
     *       // ===== Reopen and check contents
     *       arch.Open("./TestArchive.zip");
     *       cout << "Number of entries in archive: " << arch.GetNumEntries() << endl;
     *       cout << "Content of \"9.txt\": " << endl << arch.GetEntry("9.txt").GetDataAsString();
     *
     *       return 0;
     *   }
     * ```
     *
     * For further information, please refer to the full API documentation below.
     *
     * Note that the actual files in the .zip archive can be retrieved via the ZipEntry interface, not the ZipArchive interface.
     */
    class ZipArchive
    {

    public:

        /**
         * @brief Constructor. Constructs a null-archive, which can be used for creating a new .zip file
         * or opening an existing one.
         * @warning Before using an archive object that has been default constructed, a call to either Open() or Create() must be
         * performed. Otherwise, the object will be in a null-state and calls to member functions will be undefined.
         */
        explicit ZipArchive() = default;

        /**
         * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
         * it will be opened. Otherwise, a new object will be created.
         * @details
         * #### Implementation details
         * The constructors tries to open an std::ifstream. If it is valid, it means that a file already exists
         * and will be opened. Otherwise, the file does not exist and will be created.
         * @param fileName The name of the file to open or create.
         */
        explicit ZipArchive(const std::string& fileName)
                : m_ArchivePath(fileName) {

            // ===== Open file stream
            std::ifstream f(fileName.c_str());

            // ===== If successful, continue to open the file.
            if (f.good()) {
                f.close();
                Open(fileName);
            }

            // ===== If unsuccessful, create the archive file and continue.
            else {
                f.close();
                Create(fileName);
            }

        }

        /**
         * @brief Copy Constructor (deleted).
         * @param other The object to copy.
         * @note The copy constructor has been deleted, because it is not obvious what should happen to the underlying .zip file
         * when copying the archive object. Instead, if sharing of the resource is required, a std::shared_ptr can be used.
         */
        ZipArchive(const ZipArchive& other) = delete;

        /**
         * @brief Move Constructor.
         * @param other The object to be moved.
         */
        ZipArchive(ZipArchive&& other) = default;

        /**
         * @brief Destructor.
         * @note The destructor will call the Close() member function. If the archive has been modified but not saved,
         * all changes will be discarded.
         */
        virtual ~ZipArchive() {

            Close();
        }

        /**
         * @brief Copy Assignment Operator (deleted)
         * @param other The object to be copied.
         * @return A reference to the copied-to object.
         * @note The copy assignment operator has been deleted, because it is not obvious what should happen to the underlying
         * .zip file when copying the archive object. Instead, if sharing of the resource is required,
         * a std::shared_ptr can be used.
         */
        ZipArchive& operator=(const ZipArchive& other) = delete;

        /**
         * @brief Move Assignment Operator.
         * @param other The object to be moved.
         * @return A reference to the moved-to object.
         */
        ZipArchive& operator=(ZipArchive&& other) = default;

        /**
         * @brief Creates a new (empty) archive file with the given filename.
         * @details
         * #### Implementation details
         * A new archive file is created and initialized and thereafter closed, creating an empty archive file.
         * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the Open() member function.
         * @param fileName The filename for the new archive.
         */
        void Create(const std::string& fileName) {

            // ===== Prepare an archive file;
            mz_zip_archive archive = mz_zip_archive();
            mz_zip_writer_init_file(&archive, fileName.c_str(), 0);

            // ===== Finalize and close the temporary archive
            mz_zip_writer_finalize_archive(&archive);
            mz_zip_writer_end(&archive);

            // ===== Validate the temporary file
            mz_zip_error errordata;
            if (!mz_zip_validate_file_archive(fileName.c_str(), 0, &errordata))
                throw ZipRuntimeError(mz_zip_get_error_string(errordata));

            // ===== If everything is OK, open the newly created archive.
            Open(fileName);

        }

        /**
         * @brief Open an existing archive file with the given filename.
         * @details
         * ##### Implementation details
         * The archive file is opened and meta data for all the entries in the archive is loaded into memory.
         * @param fileName The filename of the archive to open.
         * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
         * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
         */
        void Open(const std::string& fileName) {

            // ===== Open the archive file for reading.
            if (m_IsOpen)
                mz_zip_reader_end(&m_Archive);
            m_ArchivePath = fileName;
            if (!mz_zip_reader_init_file(&m_Archive, m_ArchivePath.c_str(), 0))
                throw ZipRuntimeError(mz_zip_get_error_string(m_Archive.m_last_error));
            m_IsOpen = true;

            // ===== Iterate through the archive and add the entries to the internal data structure
            for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&m_Archive); i++) {
                ZipEntryInfo info;
                if (!mz_zip_reader_file_stat(&m_Archive, i, &info))
                    throw ZipRuntimeError(mz_zip_get_error_string(m_Archive.m_last_error));

                m_ZipEntries.emplace_back(Impl::ZipEntry(info));
            }

            // ===== Remove entries with identical names. The newest entries will be retained.
            auto isEqual = [](const Impl::ZipEntry& a, const Impl::ZipEntry& b) {
                return a.GetName() == b.GetName();
            };
            std::reverse(m_ZipEntries.begin(), m_ZipEntries.end());
            m_ZipEntries.erase(std::unique(m_ZipEntries.begin(), m_ZipEntries.end(), isEqual), m_ZipEntries.end());
            std::reverse(m_ZipEntries.begin(), m_ZipEntries.end());

            // ===== Add folder entries if they don't exist
            for (auto &entry : GetEntryNames(false, true)) {
                if (entry.find('/') != std::string::npos)
                    AddEntry(entry.substr(0, entry.rfind('/') + 1), "");
            }
        }

        /**
         * @brief Close the archive for reading and writing.
         * @note If the archive has been modified but not saved, all changes will be discarded.
         */
        void Close() {

            if (IsOpen())
                mz_zip_reader_end(&m_Archive);
            m_ZipEntries.clear();
            m_ArchivePath = "";
        }

        /**
         * @brief Checks if the archive file is open for reading and writing.
         * @return true if it is open; otherwise false;
         */
        bool IsOpen() const {

            return m_IsOpen;
        }

        /**
         * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names.
         */
        std::vector<std::string> GetEntryNames(bool includeDirs = true, bool includeFiles = true) const {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetEntryNames on empty ZipArchive object!");

            std::vector<std::string> result;

            // ===== Iterate through all the entries in the archive
            for (auto& item : m_ZipEntries) {

                // ===== If directories should be included and the current entry is a directory, add it to the result.
                if (includeDirs && item.IsDirectory()) {
                    result.emplace_back(item.GetName());
                    continue;
                }

                // ===== If files should be included and the current entry is a file, add it to the result.
                if (includeFiles && !item.IsDirectory()) {
                    result.emplace_back(item.GetName());
                    continue;
                }
            }

            return result;
        }

        /**
         * @brief Get a list of the entries in a specific directory of the archive. Depending on the input parameters,
         * the list will include directories, files or both.
         * @details This function behaves slightly different than the GetEntryNames() function. All entries in the
         * requested folder will be returned as absolute paths; however, only one level of subfolders will be returned.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names. The directory itself is not included.
         */
        std::vector<std::string> GetEntryNamesInDir(const std::string& dir,
                                                    bool includeDirs = true,
                                                    bool includeFiles = true) const {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetEntryNamesInDir on empty ZipArchive object!");

            // ===== Get the full list of entries
            auto result = GetEntryNames(includeDirs, includeFiles);

            // ===== Remove all entries not in the directory in question, as well as the root directory itself.
            if (!dir.empty()) {
                auto theDir = dir;
                if (theDir.back() != '/')
                    theDir += '/';

                result.erase(std::remove_if(result.begin(), result.end(), [&](const std::string& filename) {
                    return filename.substr(0, theDir.size()) != theDir || filename == theDir;
                }), result.end());
            }

            // ===== Ensure that only one level of subdirectories are included.
            result.erase(std::remove_if(result.begin(), result.end(), [&](const std::string& filename) {

                auto subFolderDepth = std::count(filename.begin(), filename.end(), '/');
                auto rootDepth      = (dir.empty() ? 1 : (dir.back() == '/' ? std::count(dir.begin(), dir.end(), '/') + 1 : std::count(dir.begin(), dir.end(), '/') + 2));
                return (subFolderDepth == rootDepth && filename.back() != '/') || (subFolderDepth > rootDepth);
            }), result.end());

            return result;
        }

        /**
         * @brief Get a list of the metadata for entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of ZipEntryMetaData structs with the entry metadata.
         */
        std::vector<ZipEntryMetaData> GetMetaData(bool includeDirs = true, bool includeFiles = true) {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetMetaData on empty ZipArchive object!");

            std::vector<ZipEntryMetaData> result;
            for (auto& item : m_ZipEntries) {
                if (includeDirs && item.IsDirectory()) {
                    result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
                    continue;
                }

                if (includeFiles && !item.IsDirectory()) {
                    result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
                    continue;
                }
            }

            return result;
        }

        /**
         * @brief Get a list of the metadata for entries in a specific directory of the archive. Depending on the input
         * parameters, the list will include directories, files or both.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of ZipEntryMetaData structs with the entry metadata. The directory itself is not included.
         */
        std::vector<ZipEntryMetaData> GetMetaDataInDir(const std::string& dir, bool includeDirs = true,
                                                       bool includeFiles = true) {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetMetaDataInDir on empty ZipArchive object!");

            std::vector<ZipEntryMetaData> result;
            for (auto& item : m_ZipEntries) {
                if (item.GetName().substr(0, dir.size()) != dir)
                    continue;

                if (includeDirs && item.IsDirectory()) {
                    result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
                    continue;
                }

                if (includeFiles && !item.IsDirectory()) {
                    result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
                    continue;
                }
            }

            return result;
        }

        /**
         * @brief Get the number of entries in the archive. Depending on the input parameters, the number will include
         * directories, files or both.
         * @param includeDirs If true, the number will include directories; otherwise not. Default is true
         * @param includeFiles If true, the number will include files; otherwise not. Default is true
         * @return An int with the number of entries.
         */
        int GetNumEntries(bool includeDirs = true, bool includeFiles = true) const {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetNumEntries on empty ZipArchive object!");

            return GetEntryNames(includeDirs, includeFiles).size();
        }

        /**
         * @brief Get the number of entries in a specific directory of the archive. Depending on the input parameters,
         * the list will include directories, files or both.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the number will include directories; otherwise not. Default is true.
         * @param includeFiles If true, the number will include files; otherwise not. Default is true.
         * @return An int with the number of entries. The directory itself is not included.
         */
        int GetNumEntriesInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true) const {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetNumEntriesInDir on empty ZipArchive object!");

            return GetEntryNamesInDir(dir, includeDirs, includeFiles).size();
        }

        /**
         * @brief Check if an entry with a given name exists in the archive.
         * @param entryName The name of the entry to check for.
         * @return true if it exists; otherwise false;
         */
        bool HasEntry(const std::string& entryName) const {

            if (!IsOpen()) throw ZipLogicError("Cannot call HasEntry on empty ZipArchive object!");

            auto result = GetEntryNames(true, true);
            return std::find(result.begin(), result.end(), entryName) != result.end();
        }

        /**
         * @brief Save the archive with a new name. The original archive will remain unchanged.
         * @param filename The new filename.
         * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
         * @throws ZipException A ZipException object is thrown if calls to miniz function fails.
         */
        void Save(std::string filename = "") {

            if (!IsOpen()) throw ZipLogicError("Cannot call Save on empty ZipArchive object!");

            if (filename.empty())
                filename = m_ArchivePath;

            // ===== Generate a random file name with the same path as the current file
            std::string tempPath = filename.substr(0, filename.rfind('/') + 1) + Impl::GenerateRandomName(20);

            // ===== Prepare an temporary archive file with the random filename;
            mz_zip_archive tempArchive = mz_zip_archive();
            mz_zip_writer_init_file(&tempArchive, tempPath.c_str(), 0);

            // ===== Iterate through the ZipEntries and add entries to the temporary file
            for (auto& file : m_ZipEntries) {
                if (file.IsDirectory()) continue; //TODO: Ensure this is the right thing to do (Excel issue)
                if (!file.IsModified()) {
                    if (!mz_zip_writer_add_from_zip_reader(&tempArchive, &m_Archive, file.Index()))
                        throw ZipRuntimeError(mz_zip_get_error_string(m_Archive.m_last_error));
                }

                else {
                    if (!mz_zip_writer_add_mem(&tempArchive,
                                               file.GetName().c_str(),
                                               file.m_EntryData.data(),
                                               file.m_EntryData.size(),
                                               MZ_DEFAULT_COMPRESSION))
                        throw ZipRuntimeError(mz_zip_get_error_string(m_Archive.m_last_error));
                }

            }

            // ===== Finalize and close the temporary archive
            mz_zip_writer_finalize_archive(&tempArchive);
            mz_zip_writer_end(&tempArchive);

            // ===== Validate the temporary file
            mz_zip_error errordata;
            if (!mz_zip_validate_file_archive(tempPath.c_str(), 0, &errordata))
                throw ZipRuntimeError(mz_zip_get_error_string(errordata));

            // ===== Close the current archive, delete the file with input filename (if it exists), rename the temporary and call Open.
            Close();
            std::remove(filename.c_str());
            std::rename(tempPath.c_str(), filename.c_str());
            Open(filename);

        }

        /**
         * @brief
         * @param stream
         */
        void Save(std::ostream& stream) {

            if (!IsOpen()) throw ZipLogicError("Cannot call Save on empty ZipArchive object!");

            // TODO: To be implemented
        }

        /**
         * @brief Deletes an entry from the archive.
         * @param name The name of the entry to delete.
         */
        void DeleteEntry(const std::string& name) {

            if (!IsOpen()) throw ZipLogicError("Cannot call DeleteEntry on empty ZipArchive object!");

            m_ZipEntries
                    .erase(std::remove_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
                        return name == entry.GetName();
                    }), m_ZipEntries.end());
        }

        /**
         * @brief Get the entry with the specified name.
         * @param name The name of the entry in the archive.
         * @return A ZipEntry object with the requested entry.
         */
        ZipEntry GetEntry(const std::string& name) {

            if (!IsOpen()) throw ZipLogicError("Cannot call GetEntry on empty ZipArchive object!");

            // ===== Look up ZipEntry object.
            auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
                return name == entry.GetName();
            });

            // ===== If data has not been extracted from the archive (i.e., m_EntryData is empty),
            // ===== extract the data from the archive to the ZipEntry object.
            if (result->m_EntryData.empty()) {
                result->m_EntryData.resize(result->UncompressedSize());
                mz_zip_reader_extract_file_to_mem(&m_Archive, name.c_str(), result->m_EntryData.data(),
                                                  result->m_EntryData.size(), 0);
            }

            // ===== Check that the operation was successful
            if (!result->IsDirectory() && result->m_EntryData.data() == nullptr)
                throw ZipRuntimeError(mz_zip_get_error_string(m_Archive.m_last_error));

            // ===== Return ZipEntry object with the file data.
            return ZipEntry(&*result);
        }

        /**
         * @brief Extract the entry with the provided name to the destination path.
         * @param name The name of the entry to extract.
         * @param dest The path to extract the entry to.
         */
        void ExtractEntry(const std::string& name, const std::string& dest) {

            if (!IsOpen()) throw ZipLogicError("Cannot call ExtractEntry on empty ZipArchive object!");

            auto entry = GetEntry(name);

            // ===== If the entry is a directory, create the directory as a subdirectory to dest
            if (entry.IsDirectory()) {
#ifdef _WIN32
                _mkdir((dest + entry.Filename()).c_str());
#else
                mkdir((dest + entry.Filename()).c_str(), 0733);
#endif
            }

            // ===== If the entry is a file, stream the entry data to a file.
            else {
                std::ofstream output(dest + "/" + entry.Filename(), std::ios::binary);
                for (auto ch : entry.GetData())
                    output << static_cast<unsigned char>(ch);
                output.close();
            }
        }

        /**
         * @brief Extract all entries in the given directory to the destination path.
         * @param dir The name of the directory to extract.
         * @param dest The path to extract the entry to.
         * @todo To be implemented
         */
        void ExtractDir(const std::string& dir, const std::string& dest) {

            if (!IsOpen()) throw ZipLogicError("Cannot call ExtractDir on empty ZipArchive object!");

        }

        /**
         * @brief Extract all archive contents to the destination path.
         * @param dest The path to extract the entry to.
         * @todo To be implemented
         */
        void ExtractAll(const std::string& dest) {

            if (!IsOpen()) throw ZipLogicError("Cannot call ExtractAll on empty ZipArchive object!");
        }

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const ZipEntryData& data) {
            return AddEntryImpl(name, data);
        }

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The std::string data to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const std::string& data) {
            ZipEntryData stringData;
            stringData.reserve(data.size());
            for (const auto & ch : data)
                stringData.emplace_back(ch);

            return AddEntryImpl(name, stringData);
        }

        /**
         * @brief Add a new entry to the archive, using another entry as input. This is mostly used for cloning existing entries
         * in the same archive, or for copying entries from one archive to another.
         * @param name The name of the entry to add.
         * @param entry The ZipEntry to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const ZipEntry& entry) {
            return AddEntryImpl(name, entry.GetData());
        }

    private:

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntryImpl(const std::string& name, const ZipEntryData& data) {

            if (!IsOpen()) throw ZipLogicError("Cannot call AddEntry on empty ZipArchive object!");

            // ===== Ensure that all folders and subfolders have an entry in the archive
            auto folders = GetEntryNames(true, false);
            auto pos = 0;
            while (name.find('/', pos) != std::string::npos) {
                pos = name.find('/', pos) + 1;
                auto folder = name.substr(0, pos);

                // ===== If folder isn't registered in the archive, add it.
                if (std::find(folders.begin(), folders.end(), folder) == folders.end()) {
                    m_ZipEntries.emplace_back(Impl::ZipEntry(folder, ""));
                    folders.emplace_back(folder);
                }
            }

            // ===== Check if an entry with the given name already exists in the archive.
            auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
                return name == entry.GetName();
            });

            // ===== If the entry exists, replace the existing data with the new data, and return the ZipEntry object.
            if (result != m_ZipEntries.end()) {
                result->SetData(data);
                return ZipEntry(&*result);
            }

            // ===== Finally, add a new entry with the given name and data, and return the object.
            return ZipEntry(&m_ZipEntries.emplace_back(Impl::ZipEntry(name, data)));
        }

    private:
        mz_zip_archive m_Archive     = mz_zip_archive(); /**< The struct used by miniz, to handle archive files. */
        std::string    m_ArchivePath = ""; /**< The path of the archive file. */
        bool           m_IsOpen      = false; /**< A flag indicating if the file is currently open for reading and writing. */

        std::vector<Impl::ZipEntry> m_ZipEntries = std::vector<Impl::ZipEntry>(); /**< Data structure for all entries in the archive. */

    };
}  // namespace Zippy

#endif //Zippy_ZIPARCHIVE_HPP
