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

#ifndef Zippy_ZIPENTRY_HPP
#define Zippy_ZIPENTRY_HPP

// ===== Standard Library Includes
#include <string>
#include <vector>
#include <iostream>

// ===== Other Includes
#include "ZipUtilities.hpp"
#include "miniz/miniz.h"

namespace Zippy
{

    class ZipArchive;
    class ZipEntry;

    // ===== Alias Declarations

    /**
     * @brief The ZipEntryInfo entity is an alias of the mz_zip_archive_file_stat from the miniz library.
     * @details The ZipEntryInfo/mz_zip_archive_file_stat struct holds various meta data related to a particular
     * entry (or item) in a zip archive, such as: comments, file size, date stamp etc.
     * @note A new ZipEntryInfo should not be created manually.
     */
    using ZipEntryInfo = mz_zip_archive_file_stat;

    /**
     * @brief The ZipEntryData entity is an alias for a std::vector of std::bytes.
     * @details This is used as a generic container of file data of any kind, both character strings and binary.
     * A vector of char or an array of char can also be used, but a vector of bytes makes it clearer that it can
     * also be used for non-text data.
     */
    using ZipEntryData = std::vector<unsigned char>;

    /**
     * @brief The ZipEntryMetaData is essentially a wrapper around the ZipEntryInfo scruct, which is an alias for a
     * miniz struct.
     */
    struct ZipEntryMetaData
    {

        /**
         * @brief Constructor.
         * @param info A reference to a ZipEntryInfo object.
         */
        explicit ZipEntryMetaData(const ZipEntryInfo& info)
                : Index(info.m_file_index),
                  CompressedSize(info.m_comp_size),
                  UncompressedSize(info.m_uncomp_size),
                  IsDirectory(info.m_is_directory),
                  IsEncrypted(info.m_is_encrypted),
                  IsSupported(info.m_is_supported),
                  Filename(info.m_filename),
                  Comment(info.m_comment),
                  Time(info.m_time) {

        }

        uint32_t     Index; /**< */
        uint64_t     CompressedSize; /**< */
        uint64_t     UncompressedSize; /**< */
        bool         IsDirectory; /**< */
        bool         IsEncrypted; /**< */
        bool         IsSupported; /**< */
        std::string  Filename; /**< */
        std::string  Comment; /**< */
        const time_t Time; /**< */
    };

    namespace Impl
    {

        /**
         * @brief The Impl::ZipEntry class implements the functionality required for manipulating entries in a zip archive.
         * @details This is the implementation class. The ZipEntry class in the Zippy namespace implements the public interface.
         * The reason for having a separate implementation class, is that it enables easy copy and move operations of
         * ZipEntry objects, without duplicating the underlying implementation objects.
         */
        class ZipEntry
        {
            friend class Zippy::ZipArchive;
            friend class Zippy::ZipEntry;

        public:

            // ===== Constructors, Destructor and Operators

            /**
             * @brief Constructor. Creates a new ZipEntry with the given ZipEntryInfo parameter. This is only used for creating
             * a ZipEntry for an entry already present in the ZipArchive, when opening an existing archive.
             * @param info A reference to a ZipEntryInfo object with the entry info.
             */
            explicit ZipEntry(const ZipEntryInfo& info)
                    : m_EntryInfo(info) {

                // ===== Call GetNewIndex to update the index counter with the value in the ZipEntryInfo object.
                GetNewIndex(info.m_file_index); // The return value is deliberately not used.
            }

            /**
             * @brief Constructor. Creates a new ZipEntry with the given name and binary data. This should only be used for creating
             * new entries, not already present in the ZipArchive
             * @param name The name of the new entry to add to the zip archive.
             * @param data A reference to a ZipEntryData object with the file data to add.
             */
            ZipEntry(const std::string& name, const ZipEntryData& data) {

                // ===== Create new ZipEntryData object
                m_EntryInfo  = CreateInfo(name);

                // ===== Assign the input data to the m_EntryData variable and set the m_IsModified flag to true.
                m_EntryData  = data;
                m_IsModified = true;
            }

            /**
             * @brief Constructor. Creates a new ZipEntry with the given name and string data. This should only be used for creating
             * new entries, not already present in the ZipArchive
             * @param name The name of the new entry to add to the zip archive.
             * @param data A string with the text data to add to the zip archive.
             */
            ZipEntry(const std::string& name, const std::string& data) {

                // ===== Create new ZipEntryData object
                m_EntryInfo = CreateInfo(name);

                // ===== Copy the string data to the m_EntryData member and set the m_IsModified flag to true.
                m_EntryData.reserve(data.size());
                for (const auto & ch : data)
                    m_EntryData.emplace_back(ch);
                m_IsModified = true;
            }

            /**
             * @brief Copy Constructor (Deleted)
             * @param other The object to copy.
             * @note The copy constructor has been deleted, because it is not obvious what should happen to the
             * underlying file when copying the entry object.
             */
            ZipEntry(const ZipEntry& other) = delete;

            /**
             * @brief Move Constructor.
             * @param other The object to be moved.
             */
            ZipEntry(ZipEntry&& other) noexcept = default;

            /**
             * @brief Destructor.
             */
            virtual ~ZipEntry() = default;

            /**
             * @brief Copy Assignment Operator (deleted)
             * @param other The object to be copied.
             * @return A reference to the copied-to object.
             * @note The copy assignment operator has been deleted, because it is not obvious what should happen to the
             * underlying file when copying the entry object.
             */
            ZipEntry& operator=(const ZipEntry& other) = delete;

            /**
             * @brief Move Assignment Operator.
             * @param other The object to be moved.
             * @return A reference to the moved-to object.
             */
            ZipEntry& operator=(ZipEntry&& other) noexcept = default;


            // ===== Data Access and Manipulation

            /**
             * @brief Get data from entry.
             * @return Returns a ZipEntryData (alias for std::vector<std::byte>) object with the fil data.
             */
            ZipEntryData GetData() const {

                return m_EntryData;
            }

            /**
             * @brief Get entry data as a std::string.
             * @return Returns a std::string with the file data.
             */
            std::string GetDataAsString() const {

                std::string result;
                for (const auto & ch : m_EntryData)
                    result += static_cast<char>(ch); // TODO: Should this use reinterpret_cast instead?

                return result;
            }

            /**
             * @brief Set the data for the entry.
             * @param data A std::string with the file data.
             */
            void SetData(const std::string& data) {

                ZipEntryData result;

                for (const auto & ch : data)
                    result.push_back(ch);

                m_EntryData  = result;
                m_IsModified = true;
            }

            /**
             * @brief Set the data for the entry.
             * @param data A ZipEntryData (alias for std::vector<std::byte>) object with the file data.
             */
            void SetData(const ZipEntryData& data) {

                m_EntryData  = data;
                m_IsModified = true;
            }

            /**
             * @brief Get the name of the entry.
             * @return Returns a std::string with the entry name.
             */
            std::string GetName() const {

                return m_EntryInfo.m_filename;
            }

            /**
             * @brief Set the name of the entry.
             * @param name A std::string with the new name of the entry.
             */
            void SetName(const std::string& name) {

#if _MSC_VER    // On MSVC, use the safe version of strcpy
                strcpy_s(m_EntryInfo.m_filename, sizeof m_EntryInfo.m_filename, name.c_str());
#else           // Otherwise, use the unsafe version as fallback :(
                strncpy(m_EntryInfo.m_filename, name.c_str(), sizeof m_EntryInfo.m_filename);
#endif
            }

            // ===== Metadata Access

            /**
             * @brief Get the entry's index in the zip archive.
             * @return Returns a uint32_t with the entry index.
             */
            uint32_t Index() const {

                return m_EntryInfo.m_file_index;
            }

            /**
             * @brief Get the uncompressed size of the entry (in bytes)
             * @return A uint64_t with the uncompressed size.
             */
            uint64_t CompressedSize() const {

                return m_EntryInfo.m_comp_size;
            }

            /**
             * @brief Get the compressed size of the entry (in bytes)
             * @return A uint64_t with the compressed size.
             */
            uint64_t UncompressedSize() const {

                return m_EntryInfo.m_uncomp_size;
            }

            /**
             * @brief Is the zip entry a directory?
             * @return Returns true if the entry is a directory; otherwise false.
             */
            bool IsDirectory() const {

                return m_EntryInfo.m_is_directory;
            }

            /**
             * @brief
             * @return Returns true if the entry is encrypted; otherwise false.
             */
            bool IsEncrypted() const {

                return m_EntryInfo.m_is_encrypted;
            }

            /**
             * @brief Is the zip entry encryption supported?
             * @return Returns true if the entry encryption (if any) is supported by the library; otherwise false.
             */
            bool IsSupported() const {

                return m_EntryInfo.m_is_supported;
            }

            /**
             * @brief Get the zip entry comments.
             * @return Returns a std::string with the comments.
             */
            std::string Comment() const {

                return m_EntryInfo.m_comment;
            }

            /**
             * @brief Get the zip entry time stamp.
             * @return Returns a time_t object with the time stamp.
             */
            const time_t& Time() const {

                return m_EntryInfo.m_time;
            }

        private:
            ZipEntryInfo m_EntryInfo = ZipEntryInfo(); /**< The zip entry metadata. */
            ZipEntryData m_EntryData = ZipEntryData(); /**< The zip entry data. */

            bool m_IsModified = false; /**< Boolean flag indicating if the file has been modified since opening. */

            /**
             * @brief Has the zip entry been modified?
             * @return Returns true if the entry is has been modified; otherwise false.
             */
            bool IsModified() const {

                return m_IsModified;
            }

            /**
             * @brief Generate a new file index.
             * @details The file index in existing zip archives may not be incrementing trivially. When opening existing
             * zip archives, this function is simply used to update the index. When adding new entries to an existing
             * archive, this function is guaranteed to provide a unique index.
             * @return Returns a uint32_t (32 bit unsigned int) with the new index.
             */
            static uint32_t GetNewIndex(uint32_t latestIndex = 0) {

                // ===== Set up a static index counter (set to zero the first time the function is executed)
                static uint32_t index{0};

                // ===== If the input value is larger than the current index value, set the index equal to the input.
                if (latestIndex > index) {
                    index = latestIndex;
                    return index;
                }

                // ===== Increment the index and return the value.
                return ++index;
            }

            /**
             * @brief Create a new ZipEntryInfo structure.
             * @details This function will create a new ZipEntryInfo structure, based on the input file name. The
             * structure values will mostly be dummy values, except for the file index, the time stamp, the file name
             * and the is_directory flag.
             * @param name The file name for the new ZipEntryInfo.
             * @return The newly created ZipEntryInfo structure is returned
             */
            static ZipEntryInfo CreateInfo(const std::string& name) {

                ZipEntryInfo info;

                info.m_file_index       = GetNewIndex(0);
                info.m_central_dir_ofs  = 0;
                info.m_version_made_by  = 0;
                info.m_version_needed   = 0;
                info.m_bit_flag         = 0;
                info.m_method           = 0;
                info.m_time             = time(nullptr);
                info.m_crc32            = 0;
                info.m_comp_size        = 0;
                info.m_uncomp_size      = 0;
                info.m_internal_attr    = 0;
                info.m_external_attr    = 0;
                info.m_local_header_ofs = 0;
                info.m_comment_size     = 0;
                info.m_is_directory     = (name.back() == '/');
                info.m_is_encrypted     = false;
                info.m_is_supported     = true;

#if _MSC_VER    // On MSVC, use the safe version of strcpy
                strcpy_s(info.m_filename, sizeof info.m_filename, name.c_str());
                strcpy_s(info.m_comment, sizeof info.m_comment, "");
#else           // Otherwise, use the unsafe version as fallback :(
                strncpy(info.m_filename, name.c_str(), sizeof info.m_filename);
                strncpy(info.m_comment, "", sizeof info.m_comment);
#endif

                return info;
            }
        };
    }  // namespace Impl

    /**
     * @brief The ZipEntry class implements the interface required for manipulating entries in a zip archive.
     * @details This is the interface class. The ZipEntry class in the Zippy::Impl namespace implements the private implementation.
     * The reason for having a separate implementation class, is that it enables easy copy and move operations of
     * ZipEntry objects, without duplicating the underlying implementation objects.
     */
    class ZipEntry
    {
        friend class ZipArchive;

    public:

        // ===== Constructors, Destructor and Operators

        /**
         * @brief Constructor. Creates a new ZipEntry.
         * @param entry A raw (non-owning) pointer to the existing Impl::ZipEntry object.
         * @todo Can this be made private?
         */
        explicit ZipEntry(Impl::ZipEntry* entry)
                : m_ZipEntry(entry) {
        }

        /**
         * @brief Copy Constructor.
         * @param other The object to be copied.
         */
        ZipEntry(const ZipEntry& other) = default;

        /**
         * @brief Move Constructor.
         * @param other The object to be moved.
         */
        ZipEntry(ZipEntry&& other) noexcept = default;

        /**
         * @brief Destructor
         */
        virtual ~ZipEntry() = default;

        /**
         * @brief Copy Assignment Operator.
         * @param other The object to be copied.
         * @return A reference to the copied-to object.
         */
        ZipEntry& operator=(const ZipEntry& other) = default;

        /**
        * @brief Move Assignment Operator.
        * @param other The object to be moved.
        * @return A reference to the moved-to object.
        */
        ZipEntry& operator=(ZipEntry&& other) noexcept = default;


        // ===== Data Access and Manipulation

        /**
         * @brief Get data from entry.
         * @return Returns a ZipEntryData (alias for std::vector<std::byte>) object with the file data.
         */
        ZipEntryData GetData() const {

            return m_ZipEntry->GetData();
        }

        /**
         * @brief Get entry data as a std::string.
         * @return Returns a std::string with the file data.
         */
        std::string GetDataAsString() const {

            return m_ZipEntry->GetDataAsString();
        }

        /**
         * @brief Set the data for the entry.
         * @param data A std::string with the file data.
         */
        void SetData(const std::string& data) {

            m_ZipEntry->SetData(data);
        }

        /**
         * @brief Set the data for the entry.
         * @param data A ZipEntryData (alias for std::vector<std::byte>) object with the file data.
         */
        void SetData(const ZipEntryData& data) {

            m_ZipEntry->SetData(data);
        }


        // ===== Metadata Access

        /**
         * @brief Get the entry's index in the zip archive.
         * @return Returns a uint32_t with the entry index.
         */
        uint32_t Index() const {

            return m_ZipEntry->Index();
        }

        /**
         * @brief Get the uncompressed size of the entry (in bytes)
         * @return A uint64_t with the uncompressed size.
         */
        uint64_t CompressedSize() const {

            return m_ZipEntry->CompressedSize();
        }

        /**
         * @brief Get the compressed size of the entry (in bytes)
         * @return A uint64_t with the compressed size.
         */
        uint64_t UncompressedSize() const {

            return m_ZipEntry->UncompressedSize();
        }

        /**
         * @brief Is the entry a directory?
         * @return Returns true if the entry is a directory; otherwise false.
         */
        bool IsDirectory() const {

            return m_ZipEntry->IsDirectory();
        }

        /**
         * @brief Is the zip entry supported?
         * @return Returns true if the entry is encrypted; otherwise false.
         */
        bool IsEncrypted() const {

            return m_ZipEntry->IsEncrypted();
        }

        /**
         * @brief Is the zip entry encryption supported?
         * @return Returns true if the entry encryption (if any) is supported; otherwise false.
         */
        bool IsSupported() const {

            return m_ZipEntry->IsSupported();
        }

        /**
         * @brief Get the entry filename.
         * @return Returns a std::string with the filename.
         * @todo Consider renaming to GetName and add a SetName member function.
         */
        std::string Filename() const {

            return m_ZipEntry->GetName();
        }

        /**
         * @brief Get the zip entry comments.
         * @return Returns a std::string with the comments.
         */
        std::string Comment() const {

            return m_ZipEntry->Comment();
        }

        /**
         * @brief Get the entry time stamp.
         * @return Returns a time_t object with the time stamp.
         */
        const time_t& Time() const {

            return m_ZipEntry->Time();
        }

    private:

        /**
         * @brief Has the file been modified?
         * @return Returns true if the entry has been modified; otherwise false.
         */
        bool IsModified() const {

            return m_ZipEntry->IsModified();
        }

        Impl::ZipEntry* m_ZipEntry;  /**< A raw (non-owning) pointer to the implementation object. */
    };
}  // namespace Zippy

#endif //Zippy_ZIPENTRY_HPP
