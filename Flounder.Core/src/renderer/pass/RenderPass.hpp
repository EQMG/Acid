#pragma once

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class RenderPass
	{
	private:
		VkRenderPass m_renderPass;
	public:
		RenderPass();

		~RenderPass();

		void Create(const VkFormat &depthFormat, const VkFormat &surfaceFormat);

		void Cleanup();

		VkRenderPass GetRenderPass() const { return m_renderPass; }
	};
}
