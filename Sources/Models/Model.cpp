#include "Model.hpp"

#include "Scenes/Scenes.hpp"
#include "Resources/Resources.hpp"

namespace acid {
bool Model::CmdRender(const CommandBuffer &commandBuffer, uint32_t instances) const {
	if (vertexBuffer && indexBuffer) {
		VkBuffer vertexBuffers[1] = {vertexBuffer->GetBuffer()};
		VkDeviceSize offsets[1] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, GetIndexType());
		vkCmdDrawIndexed(commandBuffer, indexCount, instances, 0, 0, 0);
	} else if (vertexBuffer && !indexBuffer) {
		VkBuffer vertexBuffers[1] = {vertexBuffer->GetBuffer()};
		VkDeviceSize offsets[1] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdDraw(commandBuffer, vertexCount, instances, 0, 0);
	} else {
		//throw std::runtime_error("Model with no buffers can't be rendered");
		return false;
	}

	return true;
}

std::vector<uint32_t> Model::GetIndices(std::size_t offset) const {
	Buffer indexStaging(indexBuffer->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = indexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, indexBuffer->GetBuffer(), indexStaging.GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();

	void *indicesMemory;
	indexStaging.MapMemory(&indicesMemory);
	std::vector<uint32_t> indices(indexCount);

	auto sizeOfSrcT = indexStaging.GetSize() / indexCount;

	for (uint32_t i = 0; i < indexCount; i++) {
		std::memcpy(&indices[i], static_cast<char *>(indicesMemory) + (i * sizeOfSrcT) + offset, sizeof(uint32_t));
	}

	indexStaging.UnmapMemory();
	return indices;
}

void Model::SetIndices(const std::vector<uint32_t> &indices) {
	indexBuffer = nullptr;
	indexCount = static_cast<uint32_t>(indices.size());

	if (indices.empty())
		return;
	
	Buffer indexStaging(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		indices.data());
	indexBuffer = std::make_unique<Buffer>(indexStaging.GetSize(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = indexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, indexStaging.GetBuffer(), indexBuffer->GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();
}

std::vector<float> Model::GetPointCloud() const {
	if (!vertexBuffer) return {};

	// This assumes a Vector3f attribute is the first vertex attribute.
	auto indices = GetIndices();
	auto vertices = GetVertices<Vector3f>();

	std::vector<float> pointCloud;
	pointCloud.reserve(indices.size());

	for (const auto &index : indices) {
		const auto &vertex = vertices[index];
		pointCloud.emplace_back(vertex.x);
		pointCloud.emplace_back(vertex.y);
		pointCloud.emplace_back(vertex.z);
	}

	return pointCloud;
}
}
