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
		int m_presentFamily;
		int m_computeFamily;

		std::array<uint32_t, 3> m_array;
	public:
		QueueFamilyIndices(const int &graphicsFamily, const int &presentFamily, const int &computeFamily) :
			m_graphicsFamily(graphicsFamily),
			m_presentFamily(presentFamily),
			m_computeFamily(computeFamily),
			m_array({static_cast<uint32_t>(m_graphicsFamily), static_cast<uint32_t>(m_presentFamily), static_cast<uint32_t>(m_computeFamily)})
		{
		}

		/// <summary>
		/// Gets if an index has been assigned.
		/// </summary>
		/// <return> If an index has been assigned. </return>
		bool IsComplete() const
		{
			return m_graphicsFamily >= 0 && m_presentFamily >= 0 && m_computeFamily >= 0;
		}

		int GetGraphicsFamily() const { return m_graphicsFamily; }

		int GetPresentFamily() const { return m_presentFamily; }

		int GetComputeFamily() const { return m_computeFamily; }

		std::array<uint32_t, 3> GetArray() const { return m_array; }
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
