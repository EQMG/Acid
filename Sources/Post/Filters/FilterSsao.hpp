#pragma once

#include "Post/PostFilter.hpp"
#include "Maths/Vector3.hpp"

namespace acid
{
class FilterSsao :
	public PostFilter
{
public:
	explicit FilterSsao(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	std::vector<Shader::Define> GetDefines();

	static std::shared_ptr<Image2d> ComputeNoise(const uint32_t &size);

	UniformHandler m_uniformScene;

	std::shared_ptr<Image2d> m_noise;
	std::vector<Vector3f> m_kernel;
};
}
