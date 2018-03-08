#pragma once

#include "../../Engine/Platform.hpp"
#include "RenderpassCreate.hpp"

namespace Flounder
{
	class DepthStencil;

	class F_EXPORT Renderpass
	{
	private:
		VkRenderPass m_renderPass;
	public:
		Renderpass(const RenderpassCreate &renderpassCreate, const DepthStencil &depthStencil, const VkFormat &surfaceFormat);

		~Renderpass();

		VkRenderPass GetRenderpass() const { return m_renderPass; }
	};
}
