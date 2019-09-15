#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief Class that represents a command pool.
 */
class ACID_EXPORT CommandPool
{
public:
	explicit CommandPool(const std::thread::id &threadId = std::this_thread::get_id());

	~CommandPool();

	operator const VkCommandPool &() const { return m_commandPool; }

	const VkCommandPool &GetCommandPool() const { return m_commandPool; }

	const std::thread::id &GetThreadId() const { return m_threadId; }

private:
	VkCommandPool m_commandPool = VK_NULL_HANDLE;
	std::thread::id m_threadId;
};
}
