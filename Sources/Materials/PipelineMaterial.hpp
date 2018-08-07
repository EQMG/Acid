#pragma once

#include "Models/Model.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Resources/Resources.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a material pipeline.
	/// </summary>
	class ACID_EXPORT PipelineMaterial :
		public IResource
	{
	private:
		std::string m_filename;
		Pipeline m_pipeline;
	public:
		static std::shared_ptr<PipelineMaterial> Resource(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate)
		{
			auto resource = Resources::Get()->Get(ToFilename(graphicsStage, pipelineCreate));

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<PipelineMaterial>(resource);
			}

			auto result = std::make_shared<PipelineMaterial>(graphicsStage, pipelineCreate);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		~PipelineMaterial();

		std::string GetFilename() override { return m_filename; }

		Pipeline &GetPipeline() { return m_pipeline; }

	private:
		static std::string ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);
	};
}
