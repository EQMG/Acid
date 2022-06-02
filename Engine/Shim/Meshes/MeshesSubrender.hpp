#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Shim/Export.hpp"

namespace acid {
class ACID_SHIM_EXPORT MeshesSubrender : public Subrender {
public:
	enum class Sort {
		None, Front, Back
	};

	explicit MeshesSubrender(const Pipeline::Stage &pipelineStage, Sort sort = Sort::None);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	Sort sort;
	UniformHandler uniformScene;
};
}
