#include "DescriptorsHandler.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
DescriptorsHandler::DescriptorsHandler(const Pipeline &pipeline) :
	shader(pipeline.GetShader()),
	pushDescriptors(pipeline.IsPushDescriptors()),
	descriptorSet(std::make_unique<DescriptorSet>(pipeline)),
	changed(true) {
}

void DescriptorsHandler::Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize) {
	if (shader) {
		uniformHandler.Update(shader->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler.GetUniformBuffer(), offsetSize);
	}
}

void DescriptorsHandler::Push(const std::string &descriptorName, StorageHandler &storageHandler, const std::optional<OffsetSize> &offsetSize) {
	if (shader) {
		storageHandler.Update(shader->GetUniformBlock(descriptorName));
		Push(descriptorName, storageHandler.GetStorageBuffer(), offsetSize);
	}
}

void DescriptorsHandler::Push(const std::string &descriptorName, PushHandler &pushHandler, const std::optional<OffsetSize> &offsetSize) {
	if (shader) {
		pushHandler.Update(shader->GetUniformBlock(descriptorName));
	}
}

bool DescriptorsHandler::Update(const Pipeline &pipeline) {
	if (shader != pipeline.GetShader()) {
		shader = pipeline.GetShader();
		pushDescriptors = pipeline.IsPushDescriptors();
		descriptors.clear();
		writeDescriptorSets.clear();

		if (!pushDescriptors) {
			descriptorSet = std::make_unique<DescriptorSet>(pipeline);
		}

		changed = false;
		return false;
	}

	if (changed) {
		writeDescriptorSets.clear();
		writeDescriptorSets.reserve(descriptors.size());

		for (const auto &[descriptorName, descriptor] : descriptors) {
			auto writeDescriptorSet = descriptor.writeDescriptor.GetWriteDescriptorSet();
			writeDescriptorSet.dstSet = VK_NULL_HANDLE;

			if (!pushDescriptors)
				writeDescriptorSet.dstSet = descriptorSet->GetDescriptorSet();

			writeDescriptorSets.emplace_back(writeDescriptorSet);
		}

		if (!pushDescriptors)
			descriptorSet->Update(writeDescriptorSets);

		changed = false;
	}

	return true;
}

void DescriptorsHandler::BindDescriptor(const CommandBuffer &commandBuffer, const Pipeline &pipeline) {
	if (pushDescriptors) {
		auto logicalDevice = Graphics::Get()->GetLogicalDevice();
		Instance::FvkCmdPushDescriptorSetKHR(*logicalDevice, commandBuffer, pipeline.GetPipelineBindPoint(), pipeline.GetPipelineLayout(), 0,
			static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data());
	} else {
		descriptorSet->BindDescriptor(commandBuffer);
	}
}
}
