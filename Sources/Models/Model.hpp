#pragma once

#include "Maths/Vector3.hpp"
#include "Graphics/Buffers/Buffer.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
/**
 * @brief Resource that represents a model vertex and index buffer.
 */
class ACID_EXPORT Model :
	public Resource
{
public:
	/**
	 * Creates a new model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The model with the requested values.
	 */
	static std::shared_ptr<Model> Create(const Metadata &metadata);

	/**
	 * Creates a new model, or finds one with the same values.
	 * @param filename The file to load the model from.
	 * @return The model with the requested values.
	 */
	static std::shared_ptr<Model> Create(const std::string &filename);

	/**
	 * Creates a new empty model.
	 */
	Model();

	/**
	 * Creates a new model.
	 * @tparam T The vertex type.
	 * @param vertices The model vertices.
	 * @param indices The model indices.
	 */
	template<typename T>
	explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) :
		Model()
	{
		Initialize(vertices, indices);
	}

	bool CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances = 1) const;

	void Load() override;

	std::vector<float> GetPointCloud() const;

	const Vector3f &GetMinExtents() const { return m_minExtents; }

	const Vector3f &GetMaxExtents() const { return m_maxExtents; }

	float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

	float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

	float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

	float GetRadius() const { return m_radius; }

	const Buffer *GetVertexBuffer() const { return m_vertexBuffer.get(); }

	const Buffer *GetIndexBuffer() const { return m_indexBuffer.get(); }

	const uint32_t &GetVertexCount() const { return m_vertexCount; }

	const uint32_t &GetIndexCount() const { return m_indexCount; }

	VkIndexType GetIndexType() const { return VK_INDEX_TYPE_UINT32; }

	friend const Metadata &operator>>(const Metadata &metadata, Model &model);

	friend Metadata &operator<<(Metadata &metadata, const Model &model);

protected:
	template<typename T>
	void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {})
	{
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;

		if (!vertices.empty())
		{
			auto vertexStaging = Buffer(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				vertices.data());
			m_vertexBuffer = std::make_unique<Buffer>(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			m_vertexCount = static_cast<uint32_t>(vertices.size());

			CommandBuffer commandBuffer = CommandBuffer();

			VkBufferCopy copyRegion = {};
			copyRegion.size = sizeof(T) * vertices.size();
			vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(), m_vertexBuffer->GetBuffer(), 1, &copyRegion);

			commandBuffer.SubmitIdle();
		}

		if (!indices.empty())
		{
			auto indexStaging = Buffer(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices.data());
			m_indexBuffer = std::make_unique<Buffer>(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			m_indexCount = static_cast<uint32_t>(indices.size());

			CommandBuffer commandBuffer = CommandBuffer();

			VkBufferCopy copyRegion = {};
			copyRegion.size = sizeof(uint32_t) * indices.size();
			vkCmdCopyBuffer(commandBuffer, indexStaging.GetBuffer(), m_indexBuffer->GetBuffer(), 1, &copyRegion);

			commandBuffer.SubmitIdle();
		}

		m_minExtents = Vector3f::PositiveInfinity;
		m_maxExtents = Vector3f::NegativeInfinity;

		for (const auto &vertex : vertices)
		{
			auto position = Vector3f(vertex.m_position);
			m_minExtents = m_minExtents.Min(position);
			m_maxExtents = m_maxExtents.Max(position);
		}

		m_radius = std::max(m_minExtents.Length(), m_maxExtents.Length());
	}

private:
	std::unique_ptr<Buffer> m_vertexBuffer;
	std::unique_ptr<Buffer> m_indexBuffer;
	uint32_t m_vertexCount;
	uint32_t m_indexCount;

	Vector3f m_minExtents;
	Vector3f m_maxExtents;
	float m_radius;
};
}
