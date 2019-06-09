#include "CommandPool.hpp"

#include "Graphics/Graphics.hpp"

namespace acid
{
CommandPool::CommandPool(const std::thread::id &threadId) :
	m_threadId{threadId}
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};
	auto graphicsFamily{logicalDevice->GetGraphicsFamily()};

	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;
	Graphics::CheckVk(vkCreateCommandPool(*logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));
}

CommandPool::~CommandPool()
{
	auto logicalDevice{Graphics::Get()->GetLogicalDevice()};

	vkDestroyCommandPool(*logicalDevice, m_commandPool, nullptr);
}
}
