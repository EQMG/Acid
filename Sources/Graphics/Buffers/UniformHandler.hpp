#pragma once

#include <cstring>

#include "UniformBuffer.hpp"

namespace acid {
/**
 * @brief Class that handles a uniform buffer.
 */
class ACID_EXPORT UniformHandler {
public:
	explicit UniformHandler(bool multipipeline = false);
	explicit UniformHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline = false);

	template<typename T>
	void Push(const T &object, std::size_t offset, std::size_t size) {
		if (!m_uniformBlock || !m_uniformBuffer) {
			return;
		}

		if (!m_bound) {
			m_uniformBuffer->MapMemory(&m_data);
			m_bound = true;
		}

		// If the buffer is already changed we can skip a memory comparison and just copy.
		if (m_handlerStatus == Buffer::Status::Changed || std::memcmp(static_cast<char *>(m_data), &object, size) != 0) {
			std::memcpy(static_cast<char *>(m_data) + offset, &object, size);
			m_handlerStatus = Buffer::Status::Changed;
		}
	}

	template<typename T>
	void Push(const std::string &uniformName, const T &object, std::size_t size = 0) {
		if (!m_uniformBlock || !m_uniformBuffer) {
			return;
		}

		auto uniform = m_uniformBlock->GetUniform(uniformName);

		if (!uniform) {
			return;
		}

		auto realSize = size;

		if (realSize == 0) {
			realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));
		}

		Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
	}

	bool Update(const std::optional<Shader::UniformBlock> &uniformBlock);

	const UniformBuffer *GetUniformBuffer() const { return m_uniformBuffer.get(); }

private:
	bool m_multipipeline;
	std::optional<Shader::UniformBlock> m_uniformBlock;
	uint32_t m_size = 0;
	void *m_data = nullptr;
	bool m_bound = false;
	std::unique_ptr<UniformBuffer> m_uniformBuffer;
	Buffer::Status m_handlerStatus;
};
}
