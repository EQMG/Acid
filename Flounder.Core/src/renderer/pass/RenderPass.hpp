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

		void Create(const VkDevice *logicalDevice, const VkFormat &format);

		void Cleanup(const VkDevice *logicalDevice);

		VkRenderPass GetRenderPass() const { return m_renderPass; }
	};
}
