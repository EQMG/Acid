#pragma once

#include "Display/Display.hpp"

namespace Flounder
{
	/// <summary>
	/// Used to store index of a QueueFamily with particular qualities.
	/// </summary>
	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		/// <summary>
		/// Gets if an index has been assigned.
		/// </summary>
		/// <return> If an index has been assigned. </return>
		bool IsComplete() const
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	/// <summary>
	/// A Vulkan queue family.
	/// </summary>
	class F_EXPORT QueueFamily
	{
	public:
		/// <summary>
		/// Finds a queue families of a physical device.
		/// </summary>
		/// <param name="surface"> The surface. </param>
		static QueueFamilyIndices FindQueueFamilies(const VkSurfaceKHR &surface);
	};
}
