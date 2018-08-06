#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Used to store index of a queue family with particular qualities.
	/// </summary>
	class ACID_EXPORT QueueIndices
	{
	private:
		int m_graphicsFamily;
		int m_presentFamily;
		int m_computeFamily;

		std::vector<uint32_t> m_array;
	public:
		QueueIndices(const int &graphicsFamily = -1, const int &presentFamily = -1, const int &computeFamily = -1);

		/// <summary>
		/// Finds a queue families of a physical device.
		/// </summary>
		/// <param name="surface"> The physical device. </param>
		/// <param name="surface"> The surface. </param>
		QueueIndices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);

		/// <summary>
		/// Gets if all indices have been assigned.
		/// </summary>
		/// <return> If all indices have been assigned. </return>
		bool IsComplete() const;

		int GetGraphicsFamily() const { return m_graphicsFamily; }

		int GetPresentFamily() const { return m_presentFamily; }

		int GetComputeFamily() const { return m_computeFamily; }

		std::vector<uint32_t> GetArray() const { return m_array; }
	private:
		void UpdateArray();
	};
}
