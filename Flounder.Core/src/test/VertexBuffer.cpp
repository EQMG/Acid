#include "VertexBuffer.hpp"

namespace Flounder
{
	VertexBuffer::VertexBuffer()
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		Cleanup();
	}

	void VertexBuffer::Create()
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(vertices[0]) * vertices.size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(Display::Get()->GetVkDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create vertex buffer");
		}
		else
		{
			std::cout << "Vertex buffer created successfully" << std::endl;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Display::Get()->GetVkDevice(), vertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size;
		allocateInfo.memoryTypeIndex = FindMemoryType(Display::Get()->GetVkPhysicalDevice(), memRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(Display::Get()->GetVkDevice(), &allocateInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate vertex buffer memory");
		}
		else
		{
			std::cout << "Vertex buffer memory allocated successfully" << std::endl;
		}

		vkBindBufferMemory(Display::Get()->GetVkDevice(), vertexBuffer, vertexBufferMemory, 0);

		void* data;
		vkMapMemory(Display::Get()->GetVkDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferInfo.size);
		vkUnmapMemory(Display::Get()->GetVkDevice(), vertexBufferMemory);
	}

	void VertexBuffer::Cleanup()
	{
		vkDestroyBuffer(Display::Get()->GetVkDevice(), vertexBuffer, nullptr);
		vkFreeMemory(Display::Get()->GetVkDevice(), vertexBufferMemory, nullptr);
	}

	uint32_t VertexBuffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type");
	}
}