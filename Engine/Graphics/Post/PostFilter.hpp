#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid {
/**
 * @brief Represents a post effect pipeline.
 */
class ACID_GRAPHICS_EXPORT PostFilter : public Subrender {
public:
	/**
	 * Creates a new post filter.
	 * @param pipelineStage The pipelines graphics stage.
	 * @param shaderStages The pipelines shader stages.
	 * @param defines A list of names that will be added as a define.
	 */
	PostFilter(const Pipeline::Stage &pipelineStage, const std::vector<std::filesystem::path> &shaderStages, const std::vector<Shader::Define> &defines = {});
	virtual ~PostFilter() = default;

	const DescriptorsHandler &GetDescriptorSet() const { return descriptorSet; }
	const PipelineGraphics &GetPipeline() const { return pipeline; }

	const Descriptor *GetAttachment(const std::string &descriptorName, const Descriptor *descriptor) const;
	const Descriptor *GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const;

	void SetAttachment(const std::string &descriptorName, const Descriptor *descriptor);
	bool RemoveAttachment(const std::string &name);

protected:
	/**
	 * Used instead of {@link DescriptorsHandler#Push} in instances where a writeColour is the same as samplerColour in a shader.
	 * By switching between what will be the input and output of each filter previous changes are available to the shader.
	 * @param descriptorName1 The first descriptor in the shader.
	 * @param descriptorName2 The second descriptor in the shader.
	 * @param rendererAttachment1 The name of the renderers attachment that will be first option.
	 * @param rendererAttachment2 The name of the renderers attachment that will be second option.
	 */
	void PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1, const std::string &rendererAttachment2);

	inline static uint32_t GlobalSwitching = 0;

	DescriptorsHandler descriptorSet;
	PipelineGraphics pipeline;

	std::map<std::string, const Descriptor *> attachments;
};
}
