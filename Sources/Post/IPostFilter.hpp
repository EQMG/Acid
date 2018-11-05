#pragma once

#include "Models/Model.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a FBO.
	/// </summary>
	class ACID_EXPORT IPostFilter :
		public IRenderer
	{
	protected:
		static uint32_t GLOBAL_SWITCHING;

		DescriptorsHandler m_descriptorSet;

		Pipeline m_pipeline;
		std::shared_ptr<Model> m_model;

		std::map<std::string, IDescriptor *> m_attachments;
	public:
		/// <summary>
		/// Creates a new post effect filter.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="shaderStages"> The pipelines shader stages. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		IPostFilter(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<PipelineDefine> &defines = {});

		virtual void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override = 0;

		const DescriptorsHandler &GetDescriptorSet() const { return m_descriptorSet; }

		const Pipeline &GetPipeline() const { return m_pipeline; }

		std::shared_ptr<Model> GetModel() const { return m_model; }

		IDescriptor *GetAttachment(const std::string &descriptorName, IDescriptor *descriptor) const;

		IDescriptor *GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const;

		void SetAttachment(const std::string &descriptorName, IDescriptor *descriptor);

		bool RemoveAttachment(const std::string &name);
	protected:
		/// <summary>
		/// Used instead of `m_descriptorSet.Push()` in instances where a writeColour is the same as samplerColour in a shader.
		/// By switching between what will be the input and output of each filter previous changes are available to the shader.
		/// </summary>
		/// <param name="descriptorName1"> The first descriptor in the shader. </param>
		/// <param name="descriptorName2"> The second descriptor in the shader. </param>
		/// <param name="rendererAttachment1"> The name of the renderers attachment that will be first option. </param>
		/// <param name="rendererAttachment2"> The name of the renderers attachment that will be second option. </param>
		void PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1, const std::string &rendererAttachment2);
	};
}
