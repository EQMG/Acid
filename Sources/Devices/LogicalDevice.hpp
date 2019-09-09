#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid
{
class Instance;
class PhysicalDevice;
class Surface;

class ACID_EXPORT LogicalDevice
{
public:
	LogicalDevice(const Instance *instance, const PhysicalDevice *physicalDevice, const Surface *surface);

	~LogicalDevice();

	operator const VkDevice &() const { return m_logicalDevice; }

	const VkDevice &GetLogicalDevice() const { return m_logicalDevice; }

	const VkPhysicalDeviceFeatures &GetEnabledFeatures() const { return m_enabledFeatures; }

	const VkQueue &GetGraphicsQueue() const { return m_graphicsQueue; }

	const VkQueue &GetPresentQueue() const { return m_presentQueue; }

	const VkQueue &GetComputeQueue() const { return m_computeQueue; }

	const VkQueue &GetTransferQueue() const { return m_transferQueue; }

	uint32_t GetGraphicsFamily() const { return m_graphicsFamily; }

	uint32_t GetPresentFamily() const { return m_presentFamily; }

	uint32_t GetComputeFamily() const { return m_computeFamily; }

	uint32_t GetTransferFamily() const { return m_transferFamily; }

private:
	friend class Graphics;

	void CreateQueueIndices();

	void CreateLogicalDevice();

	const Instance *m_instance;
	const PhysicalDevice *m_physicalDevice;
	const Surface *m_surface;

	VkDevice m_logicalDevice{VK_NULL_HANDLE};
	VkPhysicalDeviceFeatures m_enabledFeatures{};

	VkQueueFlags m_supportedQueues{};
	uint32_t m_graphicsFamily{};
	uint32_t m_presentFamily{};
	uint32_t m_computeFamily{};
	uint32_t m_transferFamily{};

	VkQueue m_graphicsQueue{VK_NULL_HANDLE};
	VkQueue m_presentQueue{VK_NULL_HANDLE};
	VkQueue m_computeQueue{VK_NULL_HANDLE};
	VkQueue m_transferQueue{VK_NULL_HANDLE};
};
}
