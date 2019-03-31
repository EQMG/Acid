#include "CommandPool.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
CommandPool::CommandPool(const std::thread::id &threadId) :
	m_commandPool(VK_NULL_HANDLE),
	m_threadId(threadId)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();
	auto graphicsFamily = logicalDevice->GetGraphicsFamily();

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;
	Renderer::CheckVk(vkCreateCommandPool(logicalDevice->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool));
}

CommandPool::~CommandPool()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkDestroyCommandPool(logicalDevice->GetLogicalDevice(), m_commandPool, nullptr);
}
}
