#pragma once

#include "Models/Model.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class ACID_EXPORT IPostFilter :
		public IRenderer
	{
	protected:
		DescriptorsHandler m_descriptorSet;

		Pipeline m_pipeline;
		std::shared_ptr<Model> m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="shaderStages"> The pipelines shader stages. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		IPostFilter(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<PipelineDefine> &defines = {});

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~IPostFilter();

		virtual void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override = 0;

		DescriptorsHandler GetDescriptorSet() const { return m_descriptorSet; }

		Pipeline GetPipeline() const { return m_pipeline; }

		std::shared_ptr<Model> GetModel() const { return m_model; }
	};
}
