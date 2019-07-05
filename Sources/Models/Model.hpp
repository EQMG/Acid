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
	 * @param node The node to decode values from.
	 * @return The model with the requested values.
	 */
	static std::shared_ptr<Model> Create(const Node &node);

	/**
	 * Creates a new model, or finds one with the same values.
	 * @param filename The file to load the model from.
	 * @return The model with the requested values.
	 */
	static std::shared_ptr<Model> Create(const std::filesystem::path &filename);

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

	template<typename T>
	std::vector<T> GetVertices(const std::size_t &offset = 0) const;

	template<typename T>
	void SetVertices(const std::vector<T> &vertices);

	std::vector<uint32_t> GetIndices(const std::size_t &offset = 0) const;

	void SetIndices(const std::vector<uint32_t> &indices);

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

	static VkIndexType GetIndexType() { return VK_INDEX_TYPE_UINT32; }

	friend const Node &operator>>(const Node &node, Model &model);

	friend Node &operator<<(Node &node, const Model &model);

protected:
	template<typename T>
	void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {});

private:
	std::unique_ptr<Buffer> m_vertexBuffer;
	std::unique_ptr<Buffer> m_indexBuffer;
	uint32_t m_vertexCount{};
	uint32_t m_indexCount{};

	Vector3f m_minExtents;
	Vector3f m_maxExtents;
	float m_radius{};
};
}

#include "Model.inl"
