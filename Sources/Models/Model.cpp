#include "Model.hpp"

#include "Scenes/Scenes.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<Model> Model::Create(const Metadata &metadata)
{
	return Scenes::Get()->GetModelRegister().Create(metadata);
}

std::shared_ptr<Model> Model::Create(const std::filesystem::path &filename)
{
	return Scenes::Get()->GetModelRegister().Create(filename);
}

Model::Model()
{
}

bool Model::CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances) const
{
	if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
	{
		VkBuffer vertexBuffers[]{m_vertexBuffer->GetBuffer()};
		VkDeviceSize offsets[]{0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->GetBuffer(), 0, GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_indexCount, instances, 0, 0, 0);
	}
	else if (m_vertexBuffer != nullptr && m_indexBuffer == nullptr)
	{
		VkBuffer vertexBuffers[]{m_vertexBuffer->GetBuffer()};
		VkDeviceSize offsets[]{0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdDraw(commandBuffer, m_vertexCount, instances, 0, 0);
	}
	else
	{
		//throw std::runtime_error("Model with no buffers cannot be rendered");
		return false;
	}

	return true;
}

std::vector<uint32_t> Model::GetIndices(const std::size_t &offset) const
{
	Buffer indexStaging{m_indexBuffer->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion{};
	copyRegion.size = indexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, m_indexBuffer->GetBuffer(), indexStaging.GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();

	void *indicesMemory;
	indexStaging.MapMemory(&indicesMemory);
	std::vector<uint32_t> indices(m_indexCount);

	auto sizeOfSrcT{indexStaging.GetSize() / m_indexCount};

	for (uint32_t i{}; i < m_indexCount; i++)
	{
		std::memcpy(&indices[i], static_cast<char *>(indicesMemory) + (i * sizeOfSrcT) + offset, sizeof(uint32_t));
	}

	indexStaging.UnmapMemory();
	return indices;
}

void Model::SetIndices(const std::vector<uint32_t> &indices)
{
	m_indexBuffer = nullptr;
	m_indexCount = static_cast<uint32_t>(indices.size());

	if (!indices.empty())
	{
		Buffer indexStaging{sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices.data()};
		m_indexBuffer = std::make_unique<Buffer>(indexStaging.GetSize(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CommandBuffer commandBuffer;

		VkBufferCopy copyRegion{};
		copyRegion.size = indexStaging.GetSize();
		vkCmdCopyBuffer(commandBuffer, indexStaging.GetBuffer(), m_indexBuffer->GetBuffer(), 1, &copyRegion);

		commandBuffer.SubmitIdle();
	}
}

std::vector<float> Model::GetPointCloud() const
{
	if (m_vertexBuffer == nullptr)
	{
		return {};
	}

	// This assumes a Vector3f attribute is the first vertex attribute.
	auto indices{GetIndices()};
	auto vertices{GetVertices<Vector3f>()};

	std::vector<float> pointCloud;
	pointCloud.reserve(indices.size());

	for (const auto &index : indices)
	{
		auto vertex{vertices[index]};
		pointCloud.emplace_back(vertex.m_x);
		pointCloud.emplace_back(vertex.m_y);
		pointCloud.emplace_back(vertex.m_z);
	}

	return pointCloud;
}

const Metadata &operator>>(const Metadata &metadata, Model &model)
{
	// TODO: Virtual?
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Model &model)
{
	// TODO: Virtual?
	return metadata;
}
}
