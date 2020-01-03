#pragma once

#include <exception>

#include "Export.hpp"

namespace acid {
class ACID_EXPORT ZipException : public std::runtime_error {
public:
	explicit ZipException(const std::string &err) :
		runtime_error(err) {
	}

	~ZipException() override = default;
};

class ACID_EXPORT ZipExceptionUndefined : public ZipException {
public:
	explicit ZipExceptionUndefined(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUndefined() override = default;
};

class ACID_EXPORT ZipExceptionTooManyFiles : public ZipException {
public:
	explicit ZipExceptionTooManyFiles(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionTooManyFiles() override = default;
};

class ACID_EXPORT ZipExceptionFileTooLarge : public ZipException {
public:
	explicit ZipExceptionFileTooLarge(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileTooLarge() override = default;
};

class ACID_EXPORT ZipExceptionUnsupportedMethod : public ZipException {
public:
	explicit ZipExceptionUnsupportedMethod(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUnsupportedMethod() override = default;
};

class ACID_EXPORT ZipExceptionUnsupportedEncryption : public ZipException {
public:
	explicit ZipExceptionUnsupportedEncryption(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUnsupportedEncryption() override = default;
};

class ACID_EXPORT ZipExceptionUnsupportedFeature : public ZipException {
public:
	explicit ZipExceptionUnsupportedFeature(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUnsupportedFeature() override = default;
};

class ACID_EXPORT ZipExceptionFailedFindingCentralDir : public ZipException {
public:
	explicit ZipExceptionFailedFindingCentralDir(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFailedFindingCentralDir() override = default;
};

class ACID_EXPORT ZipExceptionNotAnArchive : public ZipException {
public:
	explicit ZipExceptionNotAnArchive(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionNotAnArchive() override = default;
};

class ACID_EXPORT ZipExceptionInvalidHeader : public ZipException {
public:
	explicit ZipExceptionInvalidHeader(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionInvalidHeader() override = default;
};

class ACID_EXPORT ZipExceptionMultidiskUnsupported : public ZipException {
public:
	explicit ZipExceptionMultidiskUnsupported(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionMultidiskUnsupported() override = default;
};

class ACID_EXPORT ZipExceptionDecompressionFailed : public ZipException {
public:
	explicit ZipExceptionDecompressionFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionDecompressionFailed() override = default;
};

class ACID_EXPORT ZipExceptionCompressionFailed : public ZipException {
public:
	explicit ZipExceptionCompressionFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionCompressionFailed() override = default;
};

class ACID_EXPORT ZipExceptionUnexpectedDecompSize : public ZipException {
public:
	explicit ZipExceptionUnexpectedDecompSize(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUnexpectedDecompSize() override = default;
};

class ACID_EXPORT ZipExceptionCrcCheckFailed : public ZipException {
public:
	explicit ZipExceptionCrcCheckFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionCrcCheckFailed() override = default;
};

class ACID_EXPORT ZipExceptionUnsupportedCDirSize : public ZipException {
public:
	explicit ZipExceptionUnsupportedCDirSize(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionUnsupportedCDirSize() override = default;
};

class ACID_EXPORT ZipExceptionAllocFailed : public ZipException {
public:
	explicit ZipExceptionAllocFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionAllocFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileOpenFailed : public ZipException {
public:
	explicit ZipExceptionFileOpenFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileOpenFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileCreateFailed : public ZipException {
public:
	explicit ZipExceptionFileCreateFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileCreateFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileWriteFailed : public ZipException {
public:
	explicit ZipExceptionFileWriteFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileWriteFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileReadFailed : public ZipException {
public:
	explicit ZipExceptionFileReadFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileReadFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileCloseFailed : public ZipException {
public:
	explicit ZipExceptionFileCloseFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileCloseFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileSeekFailed : public ZipException {
public:
	explicit ZipExceptionFileSeekFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileSeekFailed() override = default;
};

class ACID_EXPORT ZipExceptionFileStatFailed : public ZipException {
public:
	explicit ZipExceptionFileStatFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileStatFailed() override = default;
};

class ACID_EXPORT ZipExceptionInvalidParameter : public ZipException {
public:
	explicit ZipExceptionInvalidParameter(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionInvalidParameter() override = default;
};

class ACID_EXPORT ZipExceptionInvalidFilename : public ZipException {
public:
	explicit ZipExceptionInvalidFilename(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionInvalidFilename() override = default;
};

class ACID_EXPORT ZipExceptionBufferTooSmall : public ZipException {
public:
	explicit ZipExceptionBufferTooSmall(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionBufferTooSmall() override = default;
};

class ACID_EXPORT ZipExceptionInternalError : public ZipException {
public:
	explicit ZipExceptionInternalError(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionInternalError() override = default;
};

class ACID_EXPORT ZipExceptionFileNotFound : public ZipException {
public:
	explicit ZipExceptionFileNotFound(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionFileNotFound() override = default;
};

class ACID_EXPORT ZipExceptionArchiveTooLarge : public ZipException {
public:
	explicit ZipExceptionArchiveTooLarge(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionArchiveTooLarge() override = default;
};

class ACID_EXPORT ZipExceptionValidationFailed : public ZipException {
public:
	explicit ZipExceptionValidationFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionValidationFailed() override = default;
};

class ACID_EXPORT ZipExceptionWriteCallbackFailed : public ZipException {
public:
	explicit ZipExceptionWriteCallbackFailed(const std::string &err) :
		ZipException(err) {
	}

	~ZipExceptionWriteCallbackFailed() override = default;
};

}
