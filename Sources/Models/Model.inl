#pragma once

#include "Model.hpp"

namespace acid {
template<typename T>
std::vector<T> Model::GetVertices(std::size_t offset) const {
	Buffer vertexStaging(m_vertexBuffer->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, m_vertexBuffer->GetBuffer(), vertexStaging.GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();

	void *verticesMemory;
	vertexStaging.MapMemory(&verticesMemory);
	std::vector<T> vertices(m_vertexCount);

	auto sizeOfSrcT = vertexStaging.GetSize() / m_vertexCount;

	for (uint32_t i = 0; i < m_vertexCount; i++) {
		std::memcpy(&vertices[i], static_cast<char *>(verticesMemory) + (i * sizeOfSrcT) + offset, sizeof(T));
	}

	vertexStaging.UnmapMemory();
	return vertices;
}

template<typename T>
void Model::SetVertices(const std::vector<T> &vertices) {
	m_vertexBuffer = nullptr;
	m_vertexCount = static_cast<uint32_t>(vertices.size());

	if (vertices.empty())
		return;
	
	Buffer vertexStaging(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertices.data());
	m_vertexBuffer = std::make_unique<Buffer>(vertexStaging.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(), m_vertexBuffer->GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();
}

template<typename T>
void Model::Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices) {
	SetVertices(vertices);
	SetIndices(indices);

	m_minExtents = Vector3f::PositiveInfinity;
	m_maxExtents = Vector3f::NegativeInfinity;

	for (const auto &vertex : vertices) {
		Vector3f position(vertex.m_position);
		m_minExtents = m_minExtents.Min(position);
		m_maxExtents = m_maxExtents.Max(position);
	}

	m_radius = std::max(m_minExtents.Length(), m_maxExtents.Length());
}
}
