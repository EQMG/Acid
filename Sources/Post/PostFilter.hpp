#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
/**
 * Represents a post effect pipeline.
 */
class ACID_EXPORT PostFilter :
	public RenderPipeline
{
public:
	/**
	 * Creates a new post filter.
	 * @param pipelineStage The pipelines graphics stage.
	 * @param shaderStages The pipelines shader stages.
	 * @param defines A list of names that will be added as a define.
	 */
	PostFilter(const Pipeline::Stage &pipelineStage, const std::vector<std::string> &shaderStages, const std::vector<Shader::Define> &defines = {});

	virtual ~PostFilter() = default;

	const DescriptorsHandler &GetDescriptorSet() const { return m_descriptorSet; }

	const PipelineGraphics &GetPipeline() const { return m_pipeline; }

	const Descriptor *GetAttachment(const std::string &descriptorName, const Descriptor *descriptor) const;

	const Descriptor *GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const;

	void SetAttachment(const std::string &descriptorName, const Descriptor *descriptor);

	bool RemoveAttachment(const std::string &name);

protected:
	static uint32_t GlobalSwitching;

	/**
	 * Used instead of {@link DescriptorsHandler#Push} in instances where a writeColour is the same as samplerColour in a shader.
	 * By switching between what will be the input and output of each filter previous changes are available to the shader.
	 * @param descriptorName1 The first descriptor in the shader.
	 * @param descriptorName2 The second descriptor in the shader.
	 * @param rendererAttachment1 The name of the renderers attachment that will be first option.
	 * @param rendererAttachment2 The name of the renderers attachment that will be second option.
	 */
	void PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1, const std::string &rendererAttachment2);

	DescriptorsHandler m_descriptorSet;
	PipelineGraphics m_pipeline;

	std::map<std::string, const Descriptor *> m_attachments;
};
}
