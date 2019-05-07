#pragma once

#include "Renderer/Subrender.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT SubrenderMeshes :
	public Subrender
{
public:
	enum class Sort
	{
		None, Front, Back
	};

	explicit SubrenderMeshes(const Pipeline::Stage &pipelineStage, const Sort &sort = Sort::None);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	Sort m_sort;
	UniformHandler m_uniformScene;
};
}
