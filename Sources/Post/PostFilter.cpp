#include "Post/PostFilter.hpp"

namespace acid {
uint32_t PostFilter::GlobalSwitching = 0;

PostFilter::PostFilter(const Pipeline::Stage &pipelineStage, const std::vector<std::filesystem::path> &shaderStages, const std::vector<Shader::Define> &defines) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, shaderStages, {}, defines, PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None) {
}

const Descriptor *PostFilter::GetAttachment(const std::string &descriptorName, const Descriptor *descriptor) const {
	if (auto it = attachments.find(descriptorName); it != attachments.end())
		return it->second;
	return descriptor;
}

const Descriptor *PostFilter::GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const {
	if (auto it = attachments.find(descriptorName); it != attachments.end())
		return it->second;
	return Graphics::Get()->GetAttachment(rendererAttachment);
}

void PostFilter::SetAttachment(const std::string &descriptorName, const Descriptor *descriptor) {
	if (auto it = attachments.find(descriptorName); it != attachments.end())
		it->second = descriptor;
	else
		attachments.emplace(descriptorName, descriptor);
}

bool PostFilter::RemoveAttachment(const std::string &name) {
	if (auto it = attachments.find(name); it != attachments.end()) {
		attachments.erase(it);
		return true;
	}

	return false;
}

void PostFilter::PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1,
	const std::string &rendererAttachment2) {
	// TODO: Clean up this state machine mess, this logic may also be incorrect.
	auto it1 = attachments.find(descriptorName1);
	auto it2 = attachments.find(descriptorName2);

	if (it1 != attachments.end() && it2 != attachments.end()) {
		descriptorSet.Push(descriptorName1, GetAttachment(descriptorName1, rendererAttachment1));
		descriptorSet.Push(descriptorName2, GetAttachment(descriptorName2, rendererAttachment1));
		return;
	}
	if (it1 == attachments.end() && it2 != attachments.end()) {
		descriptorSet.Push(descriptorName1, Graphics::Get()->GetAttachment(GlobalSwitching % 2 == 1 ? rendererAttachment1 : rendererAttachment2));
		descriptorSet.Push(descriptorName2, GetAttachment(descriptorName2, rendererAttachment1));
		return;
	}
	if (it1 != attachments.end() && it2 == attachments.end()) {
		descriptorSet.Push(descriptorName1, GetAttachment(descriptorName1, rendererAttachment1));
		descriptorSet.Push(descriptorName2, Graphics::Get()->GetAttachment(GlobalSwitching % 2 == 1 ? rendererAttachment1 : rendererAttachment2));
		return;
	}

	if (GlobalSwitching % 2 == 1) {
		descriptorSet.Push(descriptorName1, Graphics::Get()->GetAttachment(rendererAttachment1));
		descriptorSet.Push(descriptorName2, Graphics::Get()->GetAttachment(rendererAttachment2));
	} else {
		descriptorSet.Push(descriptorName1, Graphics::Get()->GetAttachment(rendererAttachment2));
		descriptorSet.Push(descriptorName2, Graphics::Get()->GetAttachment(rendererAttachment1));
	}

	GlobalSwitching++;
}
}
