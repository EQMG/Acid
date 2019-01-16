#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterSsao :
		public PostFilter
	{
	public:
		explicit FilterSsao(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		std::vector<ShaderDefine> GetDefines();

		static std::shared_ptr<acid::Texture> ComputeNoise(const uint32_t &size);
	private:
		PushHandler m_pushScene;

		std::shared_ptr<Texture> m_noise;
		std::vector<Vector3> m_kernel;
	};
}
