#include "Post/PostFilter.hpp"

namespace acid {
PostFilter::PostFilter(const Pipeline::Stage &pipelineStage, const std::vector<std::filesystem::path> &shaderStages, const std::vector<Shader::Define> &defines) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, shaderStages, {}, defines, PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None) {
}

const Descriptor *PostFilter::GetAttachment(const std::string &descriptorName, const Descriptor *descriptor) const {
	auto it = attachments.find(descriptorName);

	if (it == attachments.end()) {
		return descriptor;
	}

	return it->second;
}

const Descriptor *PostFilter::GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const {
	auto it = attachments.find(descriptorName);

	if (it == attachments.end()) {
		return Graphics::Get()->GetAttachment(rendererAttachment);
	}

	return it->second;
}

void PostFilter::SetAttachment(const std::string &descriptorName, const Descriptor *descriptor) {
	auto it = attachments.find(descriptorName);

	if (it == attachments.end()) {
		attachments.emplace(descriptorName, descriptor);
		return;
	}

	it->second = descriptor;
}

bool PostFilter::RemoveAttachment(const std::string &name) {
	auto it = attachments.find(name);

	if (it != attachments.end()) {
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
