#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterSsao :
		public PostFilter
	{
	private:
		static const uint32_t SSAO_NOISE_DIM;
		static const uint32_t SSAO_KERNEL_SIZE;
		static const float SSAO_RADIUS;

		PushHandler m_pushScene;

		std::shared_ptr<Texture> m_noise;
		std::vector<Vector3> m_kernel;
	public:
		explicit FilterSsao(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;
	private:
		std::vector<ShaderDefine> GetDefines();

		static std::shared_ptr<acid::Texture> ComputeNoise(const uint32_t &size);
	};
}
