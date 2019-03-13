#pragma once

#include "Engine/Engine.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
class Instance;
class PhysicalDevice;
class Surface;

class ACID_EXPORT LogicalDevice
{
  public:
	LogicalDevice(const Instance* instance, const PhysicalDevice* physicalDevice, const Surface* surface);

	~LogicalDevice();

	const VkDevice& GetLogicalDevice() const
	{
		return m_logicalDevice;
	}

	const VkQueue& GetGraphicsQueue() const
	{
		return m_graphicsQueue;
	}

	const VkQueue& GetPresentQueue() const
	{
		return m_presentQueue;
	}

	const VkQueue& GetComputeQueue() const
	{
		return m_computeQueue;
	}

	const VkQueue& GetTransferQueue() const
	{
		return m_transferQueue;
	}

	const uint32_t& GetGraphicsFamily() const
	{
		return m_graphicsFamily;
	}

	const uint32_t& GetPresentFamily() const
	{
		return m_presentFamily;
	}

	const uint32_t& GetComputeFamily() const
	{
		return m_computeFamily;
	}

	const uint32_t& GetTransferFamily() const
	{
		return m_transferFamily;
	}

  private:
	friend class Renderer;

	void CreateQueueIndices();

	void CreateLogicalDevice();

	const Instance* m_instance;
	const PhysicalDevice* m_physicalDevice;
	const Surface* m_surface;

	VkDevice m_logicalDevice;

	VkQueueFlags m_supportedQueues;
	uint32_t m_graphicsFamily;
	uint32_t m_presentFamily;
	uint32_t m_computeFamily;
	uint32_t m_transferFamily;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkQueue m_computeQueue;
	VkQueue m_transferQueue;
};
}
