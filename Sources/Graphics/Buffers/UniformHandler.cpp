#include "UniformHandler.hpp"

namespace acid {
UniformHandler::UniformHandler(bool multipipeline) :
	multipipeline(multipipeline),
	handlerStatus(Buffer::Status::Normal) {
}

UniformHandler::UniformHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline) :
	multipipeline(multipipeline),
	uniformBlock(uniformBlock),
	size(static_cast<uint32_t>(this->uniformBlock->GetSize())),
	uniformBuffer(std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(size))),
	handlerStatus(Buffer::Status::Normal) {
}

bool UniformHandler::Update(const std::optional<Shader::UniformBlock> &uniformBlock) {
	if (handlerStatus == Buffer::Status::Reset || (multipipeline && !this->uniformBlock) || (!multipipeline && this->uniformBlock != uniformBlock)) {
		if ((size == 0 && !this->uniformBlock) || (this->uniformBlock && this->uniformBlock != uniformBlock && static_cast<uint32_t>(this->uniformBlock->GetSize()) == size)) {
			size = static_cast<uint32_t>(uniformBlock->GetSize());
		}

		this->uniformBlock = uniformBlock;
		bound = false;
		uniformBuffer = std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(size));
		handlerStatus = Buffer::Status::Changed;
		return false;
	}

	if (handlerStatus != Buffer::Status::Normal) {
		if (bound) {
			uniformBuffer->UnmapMemory();
			bound = false;
		}

		handlerStatus = Buffer::Status::Normal;
	}

	return true;
}
}
