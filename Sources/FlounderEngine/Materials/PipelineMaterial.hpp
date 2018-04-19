#pragma once

#include "Resources/Resources.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Textures/Texture.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a material pipeline.
	/// </summary>
	class F_EXPORT PipelineMaterial :
		public IResource
	{
	private:
		std::string m_filename;
		Pipeline *m_pipeline;
	public:
		static PipelineMaterial *Resource(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<Define> &defines)
		{
			IResource *resource = Resources::Get()->Get(ToFilename(graphicsStage, pipelineCreate, defines));

			if (resource != nullptr)
			{
				return dynamic_cast<PipelineMaterial *>(resource);
			}

			PipelineMaterial *result = new PipelineMaterial(graphicsStage, pipelineCreate, defines);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<Define> &defines);

		~PipelineMaterial();

		void CmdRender(const VkCommandBuffer &commandBuffer, Model *model, DescriptorSet *descriptorSet);

		std::string GetFilename() override { return m_filename; }

		Pipeline *GetPipeline() const { return m_pipeline; }

	private:
		static std::string ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<Define> &defines);
	};
}
