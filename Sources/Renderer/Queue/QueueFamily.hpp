#pragma once

#include <array>
#include "Display/Display.hpp"

namespace fl
{
	/// <summary>
	/// Used to store index of a QueueFamily with particular qualities.
	/// </summary>
	class FL_EXPORT QueueFamilyIndices
	{
	private:
		int m_graphicsFamily;
		int m_computeFamily;
		int m_transferFamily;
		int m_sparseFamily;

		std::array<uint32_t, 4> m_array;
	public:
		QueueFamilyIndices(const int &graphicsFamily, const int &computeFamily, const int &transferFamily, const int &sparseFamily) :
			m_graphicsFamily(graphicsFamily),
			m_computeFamily(computeFamily),
			m_transferFamily(transferFamily),
			m_sparseFamily(sparseFamily),
			m_array({static_cast<uint32_t>(m_graphicsFamily), static_cast<uint32_t>(m_computeFamily), static_cast<uint32_t>(m_transferFamily), static_cast<uint32_t>(m_sparseFamily)})
		{
		}

		/// <summary>
		/// Gets if an index has been assigned.
		/// </summary>
		/// <return> If an index has been assigned. </return>
		bool IsComplete() const
		{
			return m_graphicsFamily >= 0 && m_computeFamily >= 0 && m_transferFamily >= 0 && m_sparseFamily >= 0;
		}

		int GetGraphicsFamily() const { return m_graphicsFamily; }

		int GetComputeFamily() const { return m_computeFamily; }

		int GetTransferFamily() const { return m_transferFamily; }

		int GetSparceFamily() const { return m_sparseFamily; }

		std::array<uint32_t, 4> GetArray() const { return m_array; }
	};

	/// <summary>
	/// A Vulkan queue family.
	/// </summary>
	class FL_EXPORT QueueFamily
	{
	public:
		/// <summary>
		/// Finds a queue families of a physical device.
		/// </summary>
		/// <param name="surface"> The surface. </param>
		static QueueFamilyIndices FindQueueFamilies(const VkSurfaceKHR &surface);
	};
}
