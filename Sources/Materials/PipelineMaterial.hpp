#pragma once

#include "Resources/Resources.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Textures/Texture.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a material pipeline.
	/// </summary>
	class FL_EXPORT PipelineMaterial :
		public IResource
	{
	private:
		std::string m_filename;
		Pipeline m_pipeline;
	public:
		static std::shared_ptr<PipelineMaterial> Resource(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines)
		{
			auto resource = Resources::Get()->Get(ToFilename(graphicsStage, pipelineCreate, defines));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<PipelineMaterial>(resource);
			}

			auto result = std::make_shared<PipelineMaterial>(graphicsStage, pipelineCreate, defines);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines);

		~PipelineMaterial();

		std::string GetFilename() override { return m_filename; }

		Pipeline &GetPipeline() { return m_pipeline; }

	private:
		static std::string ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines);
	};
}
