#pragma once

#include <cstring>

#include "StorageBuffer.hpp"

namespace acid {
/**
 * @brief Class that handles a storage buffer.
 */
class ACID_EXPORT
	StorageHandler {
public:
	explicit StorageHandler(bool multipipeline = false);
	explicit StorageHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline = false);

	void Push(void *data, std::size_t size) {
		if (this->size != size) {
			this->size = static_cast<uint32_t>(size);
			handlerStatus = Buffer::Status::Reset;
			return;
		}

		if (!uniformBlock || !storageBuffer)
			return;

		if (!bound) {
			storageBuffer->MapMemory(&this->data);
			bound = true;
		}

		// If the buffer is already changed we can skip a memory comparison and just copy.
		if (handlerStatus == Buffer::Status::Changed || std::memcmp(static_cast<char *>(this->data), data, size) != 0) {
			std::memcpy(static_cast<char *>(this->data), data, size);
			handlerStatus = Buffer::Status::Changed;
		}
	}

	template<typename T>
	void Push(const T &object, std::size_t offset, std::size_t size) {
		if (!uniformBlock || !storageBuffer)
			return;

		if (!bound) {
			storageBuffer->MapMemory(&this->data);
			bound = true;
		}

		// If the buffer is already changed we can skip a memory comparison and just copy.
		if (handlerStatus == Buffer::Status::Changed || std::memcmp(static_cast<char *>(this->data) + offset, &object, size) != 0) {
			std::memcpy(static_cast<char *>(this->data) + offset, &object, size);
			handlerStatus = Buffer::Status::Changed;
		}
	}

	template<typename T>
	void Push(const std::string &uniformName, const T &object, std::size_t size = 0) {
		if (!uniformBlock)
			return;

		auto uniform = uniformBlock->GetUniform(uniformName);
		if (!uniform)
			return;

		auto realSize = size;
		if (realSize == 0) {
			realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));
		}

		Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
	}

	bool Update(const std::optional<Shader::UniformBlock> &uniformBlock);

	const StorageBuffer *GetStorageBuffer() const { return storageBuffer.get(); }

private:
	bool multipipeline;
	std::optional<Shader::UniformBlock> uniformBlock;
	uint32_t size = 0;
	void *data = nullptr;
	bool bound = false;
	std::unique_ptr<StorageBuffer> storageBuffer;
	Buffer::Status handlerStatus;
};
}
