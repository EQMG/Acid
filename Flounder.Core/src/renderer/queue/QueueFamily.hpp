#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	/// <summary>
	/// Used to store index of a QueueFamily with particular qualities.
	/// </summary>
	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int transferFamily = -1;

		/// <summary>
		/// Gets if an index has been assigned.
		/// </summary>
		/// <return> If an index has been assigned. </return>
		bool IsComplete() const
		{
			return graphicsFamily >= 0 && transferFamily >= 0;
		}
	};

	/// <summary>
	/// A Vulkan queue family.
	/// </summary>
	class QueueFamily
	{
	public:
		/// <summary>
		/// Finds a queue families of a physical device.
		/// </summary>
		/// <param name="physicalDevice"> The physical device. </param>
		/// <param name="surface"> The surface. </param>
		static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);
	};
}
