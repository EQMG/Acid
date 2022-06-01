#pragma once

#include <cstring>

#include "UniformBuffer.hpp"

namespace acid {
/**
 * @brief Class that handles a uniform buffer.
 */
class ACID_GRAPHICS_EXPORT UniformHandler {
public:
	explicit UniformHandler(bool multipipeline = false);
	explicit UniformHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline = false);

	template<typename T>
	void Push(const T &object, std::size_t offset, std::size_t size) {
		if (!uniformBlock || !uniformBuffer)
			return;

		if (!bound) {
			uniformBuffer->MapMemory(&this->data);
			bound = true;
		}

		// If the buffer is already changed we can skip a memory comparison and just copy.
		if (handlerStatus == Buffer::Status::Changed || std::memcmp(static_cast<char *>(this->data), &object, size) != 0) {
			std::memcpy(static_cast<char *>(this->data) + offset, &object, size);
			handlerStatus = Buffer::Status::Changed;
		}
	}

	template<typename T>
	void Push(const std::string &uniformName, const T &object, std::size_t size = 0) {
		if (!uniformBlock || !uniformBuffer)
			return;

		auto uniform = uniformBlock->GetUniform(uniformName);
		if (!uniform)
			return;

		auto realSize = size;
		if (realSize == 0)
			realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));

		Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
	}

	bool Update(const std::optional<Shader::UniformBlock> &uniformBlock);

	const UniformBuffer *GetUniformBuffer() const { return uniformBuffer.get(); }

private:
	bool multipipeline;
	std::optional<Shader::UniformBlock> uniformBlock;
	uint32_t size = 0;
	void *data = nullptr;
	bool bound = false;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	Buffer::Status handlerStatus;
};
}
