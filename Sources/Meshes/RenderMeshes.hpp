#pragma once

#include "Renderer/Render.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RenderMeshes :
	public Render
{
public:
	enum class Sort
	{
		None, Front, Back
	};

	explicit RenderMeshes(const Pipeline::Stage &pipelineStage, const Sort &sort = Sort::None);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	Sort m_sort;
	UniformHandler m_uniformScene;
};
}
