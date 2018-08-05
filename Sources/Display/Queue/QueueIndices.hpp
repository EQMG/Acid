#pragma once

#include <array>
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
		int m_computeFamily;
		int m_transferFamily;
		int m_sparseFamily;

		std::array<uint32_t, 4> m_array;
	public:
		QueueIndices(const int &graphicsFamily = -1, const int &computeFamily = -1, const int &transferFamily = -1, const int &sparseFamily = -1);

		/// <summary>
		/// Finds a queue families of a physical device.
		/// </summary>
		/// <param name="surface"> The surface. </param>
		QueueIndices(const VkPhysicalDevice &physicalDevice);

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

		int GetComputeFamily() const { return m_computeFamily; }

		int GetTransferFamily() const { return m_transferFamily; }

		int GetSparceFamily() const { return m_sparseFamily; }

		std::array<uint32_t, 4> GetArray() const { return m_array; }
	};
}
