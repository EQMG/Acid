#pragma once

#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderPipeline.hpp"

namespace acid
{
class ACID_EXPORT RendererMeshes : public RenderPipeline
{
  public:
	enum class Sort
	{
		None,
		Front,
		Back
	};

	explicit RendererMeshes(const Pipeline::Stage& pipelineStage, const Sort& sort = Sort::None);

	void Render(const CommandBuffer& commandBuffer) override;

  private:
	Sort m_sort;
	UniformHandler m_uniformScene;
};
}
