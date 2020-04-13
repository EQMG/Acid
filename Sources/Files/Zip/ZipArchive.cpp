#include "ZipArchive.hpp"

#include <algorithm>
#include <fstream>
#include <random>

#include "Utils/String.hpp"
#include "ZipException.hpp"

namespace acid {
ZipArchive::ZipArchive(const std::filesystem::path &filename) :
	archivePath(filename) {
	std::ifstream f(filename.c_str());
	if (f.good()) {
		f.close();
		Open(filename);
	} else {
		f.close();
		Create(filename);
	}
}

ZipArchive::~ZipArchive() {
	Close();
}

void ZipArchive::Create(const std::filesystem::path &filename) {
	auto filenameU8 = filename.u8string();

	// Prepare an archive file;
	mz_zip_archive archive = {};
	mz_zip_writer_init_file(&archive, filenameU8.c_str(), 0);

	// Finalize and close the temporary archive
	mz_zip_writer_finalize_archive(&archive);
	mz_zip_writer_end(&archive);

	// Validate the temporary file
	mz_zip_error errorData;
	if (!mz_zip_validate_file_archive(filenameU8.c_str(), 0, &errorData))
		ThrowException(errorData, "Archive creation failed!");

	Open(filename);
}

void ZipArchive::Open(const std::filesystem::path &filename) {
	// Open the archive file for reading.
	if (isOpen)
		mz_zip_reader_end(&archive);
	archivePath = filename;
	auto archivePathU8 = archivePath.u8string();
	if (!mz_zip_reader_init_file(&archive, archivePathU8.c_str(), 0))
		ThrowException(archive.m_last_error, "Error opening archive file " + archivePathU8 + ".");
	isOpen = true;

	// Iterate through the archive and add the entries to the internal data structure
	for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&archive); i++) {
		ZipEntryInfo info;
		if (!mz_zip_reader_file_stat(&archive, i, &info))
			ThrowException(archive.m_last_error, "Failed getting entry info.");
		zipEntries.emplace_back(std::make_unique<ZipEntry>(info));
	}

	// Remove entries with identical names. The newest entries will be retained.
	auto isEqual = [](const auto &a, const auto &b) { return a->GetFilename() == b->GetFilename(); };
	std::reverse(zipEntries.begin(), zipEntries.end());
	zipEntries.erase(std::unique(zipEntries.begin(), zipEntries.end(), isEqual), zipEntries.end());
	std::reverse(zipEntries.begin(), zipEntries.end());
}

void ZipArchive::Close() {
	if (isOpen)
		mz_zip_reader_end(&archive);
	zipEntries.clear();
	archivePath = "";
}

std::vector<std::string> ZipArchive::GetEntryNames(bool includeDirs, bool includeFiles) const {
	std::vector<std::string> result;
	for (const auto &item : zipEntries) {
		if (includeDirs && item->IsDirectory()) {
			result.emplace_back(item->GetFilename());
			continue;
		}

		if (includeFiles && !item->IsDirectory()) {
			result.emplace_back(item->GetFilename());
			continue;
		}
	}
	return result;
}

std::vector<std::string> ZipArchive::GetEntryNamesInDir(const std::string &dir, bool includeDirs, bool includeFiles) const {
	auto result = GetEntryNames(includeDirs, includeFiles);

	if (dir.empty())
		return result;

	auto theDir = dir;
	if (theDir.back() != '/')
		theDir += '/';

	result.erase(std::remove_if(result.begin(), result.end(), [&](const std::string &filename) {
		return filename.substr(0, theDir.size()) != theDir || filename == theDir;
	}), result.end());

	return result;
}

std::vector<ZipEntryMetaData> ZipArchive::GetMetaData(bool includeDirs, bool includeFiles) const {
	std::vector<ZipEntryMetaData> result;
	for (const auto &item : zipEntries) {
		if (includeDirs && item->IsDirectory()) {
			result.emplace_back(ZipEntryMetaData(item->entryInfo));
			continue;
		}

		if (includeFiles && !item->IsDirectory()) {
			result.emplace_back(ZipEntryMetaData(item->entryInfo));
			continue;
		}
	}

	return result;
}

std::vector<ZipEntryMetaData> ZipArchive::GetMetaDataInDir(const std::string &dir, bool includeDirs, bool includeFiles) const {
	std::vector<ZipEntryMetaData> result;
	for (const auto &item : zipEntries) {
		if (item->GetFilename().substr(0, dir.size()) != dir)
			continue;

		if (includeDirs && item->IsDirectory()) {
			result.emplace_back(ZipEntryMetaData(item->entryInfo));
			continue;
		}

		if (includeFiles && !item->IsDirectory()) {
			result.emplace_back(ZipEntryMetaData(item->entryInfo));
			continue;
		}
	}

	return result;
}

int ZipArchive::GetNumEntries(bool includeDirs, bool includeFiles) const {
	return GetEntryNames(includeDirs, includeFiles).size();
}

int ZipArchive::GetNumEntriesInDir(const std::string &dir, bool includeDirs, bool includeFiles) const {
	return GetEntryNamesInDir(dir, includeDirs, includeFiles).size();
}

bool ZipArchive::HasEntry(const std::string &entryName) const {
	auto result = GetEntryNames(true, true);
	return std::find(result.begin(), result.end(), entryName) != result.end();
}

void ZipArchive::Write(std::filesystem::path filename) {
	if (filename.empty())
		filename = archivePath;

	// Generate a random file name with the same path as the current file
	auto tempPath = filename.parent_path() / GenerateRandomName(20);
	auto tempPathU8 = tempPath.u8string();

	// Prepare an temporary archive file with the random filename;
	mz_zip_archive tempArchive = {};
	mz_zip_writer_init_file(&tempArchive, tempPathU8.c_str(), 0);

	// Iterate through the ZipEntries and add entries to the temporary file
	for (const auto &file : zipEntries) {
		if (!file->IsModified()) {
			if (!mz_zip_writer_add_from_zip_reader(&tempArchive, &archive, file->GetIndex()))
				ThrowException(archive.m_last_error, "Failed copying archive entry.");
		} else {
			if (!mz_zip_writer_add_mem(&tempArchive, file->GetFilename().c_str(), file->entryData.data(), file->entryData.size(), MZ_DEFAULT_COMPRESSION))
				ThrowException(archive.m_last_error, "Failed adding archive entry.");
		}
	}

	// Finalize and close the temporary archive
	mz_zip_writer_finalize_archive(&tempArchive);
	mz_zip_writer_end(&tempArchive);

	// Validate the temporary file
	mz_zip_error errordata;
	if (!mz_zip_validate_file_archive(tempPathU8.c_str(), 0, &errordata))
		ThrowException(errordata, "Invalid archive");

	// Close the current archive, delete the file with input filename (if it exists), rename the temporary and call Open.
	Close();
	std::filesystem::remove(filename);
	std::filesystem::rename(tempPath, filename);
	Open(filename);

}

void ZipArchive::Write(std::ostream &stream) {
	// TODO: Implement
}

void ZipArchive::DeleteEntry(const std::string &name) {
	zipEntries.erase(std::remove_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	}), zipEntries.end());
}

ZipEntry *ZipArchive::GetEntry(const std::string &name) {
	// Look up ZipEntry object.
	const auto &result = std::find_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	});

	// Extract the data from the archive to the ZipEntry object.
	(*result)->entryData.resize(static_cast<std::size_t>((*result)->GetUncompressedSize()));
	mz_zip_reader_extract_file_to_mem(&archive, name.c_str(), (*result)->entryData.data(), (*result)->entryData.size(), 0);

	// Check that the operation was successful
	if (!(*result)->entryData.data())
		ThrowException(archive.m_last_error, "Error extracting archive entry.");

	// Return ZipEntry object with the file data.
	return (*result).get();
}

void ZipArchive::ExtractEntry(const std::string &name, const std::filesystem::path &dest) {
	// Look up ZipEntry object.
	const auto &result = std::find_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	});

	if (auto parentPath = dest.parent_path(); !parentPath.empty()) {
		std::filesystem::create_directories(parentPath);
	}

	auto destU8 = dest.u8string();

	// Extract the data from the archive to the ZipEntry object.
	(*result)->entryData.resize((*result)->GetUncompressedSize());
	mz_zip_reader_extract_file_to_file(&archive, name.c_str(), destU8.c_str(), 0);

	// Check that the operation was successful
	if (!(*result)->entryData.data())
		ThrowException(archive.m_last_error, "Error extracting archive entry.");
}

void ZipArchive::ExtractDir(const std::string &dir, const std::filesystem::path &dest) {
	for (const auto &entry : GetEntryNames(false)) {
		if (String::StartsWith(entry, dir))
			ExtractEntry(entry, dest / String::ReplaceFirst(entry, dir + "/", ""));
	}
}

void ZipArchive::ExtractAll(const std::filesystem::path &dest) {
	for (const auto &entry : GetEntryNames(false)) {
		ExtractEntry(entry, dest / entry);
	}
}

ZipEntry *ZipArchive::AddEntry(const std::string &name, const ZipEntryData &data) {
	// Check if an entry with the given name already exists in the archive.
	auto result = std::find_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	});

	// If the entry exists, replace the existing data with the new data, and return the ZipEntry object.
	if (result != zipEntries.end()) {
		(*result)->SetData(data);
		return (*result).get();
	}

	// Otherwise, add a new entry with the given name and data, and return the object.
	return zipEntries.emplace_back(std::make_unique<ZipEntry>(name, data)).get();
}

ZipEntry *ZipArchive::AddEntry(const std::string &name, const std::string &data) {
	// Check if an entry with the given name already exists in the archive.
	auto result = std::find_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	});

	// If the entry exists, replace the existing data with the new data, and return the ZipEntry object.
	if (result != zipEntries.end()) {
		(*result)->SetData(data);
		return (*result).get();
	}

	// Otherwise, add a new entry with the given name and data, and return the object.
	return zipEntries.emplace_back(std::make_unique<ZipEntry>(name, data)).get();
}

ZipEntry *ZipArchive::AddEntry(const std::string &name, const ZipEntry &entry) {
	// TODO: Ensure to check for self-asignment.
	// Check if an entry with the given name already exists in the archive.
	auto result = std::find_if(zipEntries.begin(), zipEntries.end(), [&](const auto &entry) {
		return name == entry->GetFilename();
	});

	// If the entry exists, replace the existing data with the new data, and return the ZipEntry object.
	if (result != zipEntries.end()) {
		(*result)->SetData(entry.GetData());
		return (*result).get();
	}

	// Otherwise, add a new entry with the given name and data, and return the object.
	return zipEntries.emplace_back(std::make_unique<ZipEntry>(name, entry.GetData())).get();
}

void ZipArchive::ThrowException(mz_zip_error error, const std::string &errorString) {
	switch (error) {
	case MZ_ZIP_UNDEFINED_ERROR:
		throw ZipExceptionUndefined(errorString);
	case MZ_ZIP_TOO_MANY_FILES:
		throw ZipExceptionTooManyFiles(errorString);
	case MZ_ZIP_FILE_TOO_LARGE:
		throw ZipExceptionFileTooLarge(errorString);
	case MZ_ZIP_UNSUPPORTED_METHOD:
		throw ZipExceptionUnsupportedMethod(errorString);
	case MZ_ZIP_UNSUPPORTED_ENCRYPTION:
		throw ZipExceptionUnsupportedEncryption(errorString);
	case MZ_ZIP_UNSUPPORTED_FEATURE:
		throw ZipExceptionUnsupportedFeature(errorString);
	case MZ_ZIP_FAILED_FINDING_CENTRAL_DIR:
		throw ZipExceptionFailedFindingCentralDir(errorString);
	case MZ_ZIP_NOT_AN_ARCHIVE:
		throw ZipExceptionNotAnArchive(errorString);
	case MZ_ZIP_INVALID_HEADER_OR_CORRUPTED:
		throw ZipExceptionInvalidHeader(errorString);
	case MZ_ZIP_UNSUPPORTED_MULTIDISK:
		throw ZipExceptionMultidiskUnsupported(errorString);
	case MZ_ZIP_DECOMPRESSION_FAILED:
		throw ZipExceptionDecompressionFailed(errorString);
	case MZ_ZIP_COMPRESSION_FAILED:
		throw ZipExceptionCompressionFailed(errorString);
	case MZ_ZIP_UNEXPECTED_DECOMPRESSED_SIZE:
		throw ZipExceptionUnexpectedDecompSize(errorString);
	case MZ_ZIP_CRC_CHECK_FAILED:
		throw ZipExceptionCrcCheckFailed(errorString);
	case MZ_ZIP_UNSUPPORTED_CDIR_SIZE:
		throw ZipExceptionUnsupportedCDirSize(errorString);
	case MZ_ZIP_ALLOC_FAILED:
		throw ZipExceptionAllocFailed(errorString);
	case MZ_ZIP_FILE_OPEN_FAILED:
		throw ZipExceptionFileOpenFailed(errorString);
	case MZ_ZIP_FILE_CREATE_FAILED:
		throw ZipExceptionFileCreateFailed(errorString);
	case MZ_ZIP_FILE_WRITE_FAILED:
		throw ZipExceptionFileWriteFailed(errorString);
	case MZ_ZIP_FILE_READ_FAILED:
		throw ZipExceptionFileReadFailed(errorString);
	case MZ_ZIP_FILE_CLOSE_FAILED:
		throw ZipExceptionFileCloseFailed(errorString);
	case MZ_ZIP_FILE_SEEK_FAILED:
		throw ZipExceptionFileSeekFailed(errorString);
	case MZ_ZIP_FILE_STAT_FAILED:
		throw ZipExceptionFileStatFailed(errorString);
	case MZ_ZIP_INVALID_PARAMETER:
		throw ZipExceptionInvalidParameter(errorString);
	case MZ_ZIP_INVALID_FILENAME:
		throw ZipExceptionInvalidFilename(errorString);
	case MZ_ZIP_BUF_TOO_SMALL:
		throw ZipExceptionBufferTooSmall(errorString);
	case MZ_ZIP_INTERNAL_ERROR:
		throw ZipExceptionInternalError(errorString);
	case MZ_ZIP_FILE_NOT_FOUND:
		throw ZipExceptionFileNotFound(errorString);
	case MZ_ZIP_ARCHIVE_TOO_LARGE:
		throw ZipExceptionArchiveTooLarge(errorString);
	case MZ_ZIP_VALIDATION_FAILED:
		throw ZipExceptionValidationFailed(errorString);
	case MZ_ZIP_WRITE_CALLBACK_FAILED:
		throw ZipExceptionWriteCallbackFailed(errorString);
	default:
		throw ZipException(errorString);
	}
}

std::string ZipArchive::GenerateRandomName(int length) {
	static std::string_view letters = "abcdefghijklmnopqrstuvwxyz0123456789";

	int32_t rangeFrom = 0;
	int32_t rangeTo = letters.size() - 1;
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_int_distribution<int32_t> distr(rangeFrom, rangeTo);

	std::string result;
	for (int i = 0; i < length; ++i) {
		result += letters[distr(generator)];
	}

	return result + ".tmp";
}
}
