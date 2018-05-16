#pragma once

#include "RenderpassCreate.hpp"

namespace fl
{
	class DepthStencil;

	class FL_HIDDEN Renderpass
	{
	private:
		VkRenderPass m_renderPass;
	public:
		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat);

		~Renderpass();

		VkRenderPass GetVkRenderpass() const { return m_renderPass; }
	};
}
