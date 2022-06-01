#pragma once

#include <cstring>

#include "Graphics/Pipelines/Pipeline.hpp"

namespace acid {
/**
 * @brief Class that handles a pipeline push constant.
 */
class ACID_GRAPHICS_EXPORT PushHandler {
public:
	explicit PushHandler(bool multipipeline = false);
	explicit PushHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline = false);

	template<typename T>
	void Push(const T &object, std::size_t offset, std::size_t size) {
		std::memcpy(data.get() + offset, &object, size);
	}

	template<typename T>
	void Push(const std::string &uniformName, const T &object, std::size_t size = 0) {
		if (!uniformBlock) {
			return;
		}

		auto uniform = uniformBlock->GetUniform(uniformName);

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

	void BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline);

private:
	bool multipipeline;
	std::optional<Shader::UniformBlock> uniformBlock;
	std::unique_ptr<char[]> data;
};
}
