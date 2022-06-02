#pragma once

#include "Utils/Future.hpp"
#include "Post/PostFilter.hpp"
#include "Maths/Vector3.hpp"
#include "Shim/Export.hpp"

namespace acid {
class ACID_SHIM_EXPORT SsaoFilter : public PostFilter {
public:
	explicit SsaoFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	std::vector<Shader::Define> GetDefines() const;

	static std::shared_ptr<Image2d> ComputeNoise(uint32_t size);

	UniformHandler uniformScene;

	Future<std::shared_ptr<Image2d>> noise;
	std::vector<Vector3f> kernel;
};
}
