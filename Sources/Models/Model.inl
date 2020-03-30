#pragma once

#include <cstring>

#include "Model.hpp"

namespace acid {
template<typename T>
Model::Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices):
	Model() {
	Initialize(vertices, indices);
}

template<typename T>
std::vector<T> Model::GetVertices(std::size_t offset) const {
	Buffer vertexStaging(vertexBuffer->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, vertexBuffer->GetBuffer(), vertexStaging.GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();

	void *verticesMemory;
	vertexStaging.MapMemory(&verticesMemory);
	std::vector<T> vertices(vertexCount);

	auto sizeOfSrcT = vertexStaging.GetSize() / vertexCount;

	for (uint32_t i = 0; i < vertexCount; i++) {
		std::memcpy(&vertices[i], static_cast<char *>(verticesMemory) + (i * sizeOfSrcT) + offset, sizeof(T));
	}

	vertexStaging.UnmapMemory();
	return vertices;
}

template<typename T>
void Model::SetVertices(const std::vector<T> &vertices) {
	vertexBuffer = nullptr;
	vertexCount = static_cast<uint32_t>(vertices.size());

	if (vertices.empty())
		return;
	
	Buffer vertexStaging(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertices.data());
	vertexBuffer = std::make_unique<Buffer>(vertexStaging.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(), vertexBuffer->GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();
}

template<typename T>
void Model::Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices) {
	SetVertices(vertices);
	SetIndices(indices);

	minExtents = Vector3f::Infinity;
	maxExtents = -Vector3f::Infinity;

	for (const auto &vertex : vertices) {
		Vector3f position(vertex.position);
		minExtents = minExtents.Min(position);
		maxExtents = maxExtents.Max(position);
	}

	radius = std::max(minExtents.Length(), maxExtents.Length());
}
}
