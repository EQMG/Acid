#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterSSAO :
		public PostFilter
	{
	public:
		explicit FilterSSAO(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		std::vector<Shader::Define> GetDefines();

		static std::shared_ptr<Texture> ComputeNoise(const uint32_t &size);

		UniformHandler m_uniformScene;

		std::shared_ptr<Texture> m_noise;
		std::vector<Vector3> m_kernel;
	};
}
