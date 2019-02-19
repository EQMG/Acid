#pragma once

#include "Models/Model.hpp"
#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a FBO.
	/// </summary>
	class ACID_EXPORT PostFilter :
		public RenderPipeline
	{
	public:
		/// <summary>
		/// Creates a new post effect filter.
		/// </summary>
		/// <param name="pipelineStage"> The pipelines graphics stage. </param>
		/// <param name="shaderStages"> The pipelines shader stages. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		PostFilter(const Pipeline::Stage &pipelineStage, const std::vector<std::string> &shaderStages, const std::vector<Shader::Define> &defines = {});

		const DescriptorsHandler &GetDescriptorSet() const { return m_descriptorSet; }

		const PipelineGraphics &GetPipeline() const { return m_pipeline; }

		const std::shared_ptr<Model> &GetModel() const { return m_model; }

		const Descriptor *GetAttachment(const std::string &descriptorName, const Descriptor *descriptor) const;

		const Descriptor *GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const;

		void SetAttachment(const std::string &descriptorName, const Descriptor *descriptor);

		bool RemoveAttachment(const std::string &name);

	protected:
		static uint32_t GlobalSwitching;

		/// <summary>
		/// Used instead of `m_descriptorSet.Push()` in instances where a writeColour is the same as samplerColour in a shader.
		/// By switching between what will be the input and output of each filter previous changes are available to the shader.
		/// </summary>
		/// <param name="descriptorName1"> The first descriptor in the shader. </param>
		/// <param name="descriptorName2"> The second descriptor in the shader. </param>
		/// <param name="rendererAttachment1"> The name of the renderers attachment that will be first option. </param>
		/// <param name="rendererAttachment2"> The name of the renderers attachment that will be second option. </param>
		void PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1, const std::string &rendererAttachment2);

		DescriptorsHandler m_descriptorSet;
		PipelineGraphics m_pipeline;
		std::shared_ptr<Model> m_model;

		std::map<std::string, const Descriptor *> m_attachments;
	};
}
