#include "PushHandler.hpp"

namespace acid {
PushHandler::PushHandler(bool multipipeline) :
	multipipeline(multipipeline) {
}

PushHandler::PushHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline) :
	multipipeline(multipipeline),
	uniformBlock(uniformBlock),
	data(std::make_unique<char[]>(this->uniformBlock->GetSize())) {
}

bool PushHandler::Update(const std::optional<Shader::UniformBlock> &uniformBlock) {
	if ((multipipeline && !this->uniformBlock) || (!multipipeline && this->uniformBlock != uniformBlock)) {
		this->uniformBlock = uniformBlock;
		data = std::make_unique<char[]>(this->uniformBlock->GetSize());
		return false;
	}

	return true;
}

void PushHandler::BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline) {
	vkCmdPushConstants(commandBuffer, pipeline.GetPipelineLayout(), uniformBlock->GetStageFlags(), 0, static_cast<uint32_t>(uniformBlock->GetSize()), data.get());
}
}
