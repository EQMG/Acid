#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class SubrenderMeshes :
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
