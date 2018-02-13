#pragma once

#include "../../Engine/Platform.hpp"

namespace Flounder
{
	class F_EXPORT RenderPass
	{
	private:
		VkRenderPass m_renderPass;
	public:
		RenderPass(const VkFormat &depthFormat, const VkFormat &surfaceFormat);

		~RenderPass();

		VkRenderPass GetRenderPass() const { return m_renderPass; }
	};
}
