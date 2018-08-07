#pragma once

#include "RenderpassCreate.hpp"

namespace acid
{
	class DepthStencil;

	class ACID_EXPORT Renderpass
	{
	private:
		VkRenderPass m_renderPass;
	public:
		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Renderpass();

		VkRenderPass GetRenderpass() const { return m_renderPass; }
	};
}
