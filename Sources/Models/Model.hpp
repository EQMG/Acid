#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include "Maths/Vector3.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Resources/Resource.hpp"
#include "IVertex.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class ACID_EXPORT Model :
		public Resource
	{
	public:
		static std::shared_ptr<Model> Create(const Metadata &metadata);

		static std::shared_ptr<Model> Create(const std::string &filename);

		/// <summary>
		/// Creates a new empty model.
		/// </summary>
		Model();

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="T"> The vertex class that implements <seealso cref="IVertex"/>. </param>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="indices"> The model indices. </param>
		template<typename T>
		explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) :
			Model()
		{
			Initialize(vertices, indices);
		}

		bool CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances = 1) const;

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		std::vector<float> GetPointCloud() const;

		const Vector3 &GetMinExtents() const { return m_minExtents; }

		const Vector3 &GetMaxExtents() const { return m_maxExtents; }

		float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

		float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

		float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

		float GetRadius() const { return m_radius; }

		const Buffer *GetVertexBuffer() const { return m_vertexBuffer.get(); }

		const Buffer *GetIndexBuffer() const { return m_indexBuffer.get(); }

		const uint32_t &GetVertexCount() const { return m_vertexCount; }

		const uint32_t &GetIndexCount() const { return m_indexCount; }

		VkIndexType GetIndexType() const { return VK_INDEX_TYPE_UINT32; }
	protected:
		template<typename T>
		void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {})
		{
			static_assert(std::is_base_of<IVertex, T>::value, "T must derive from IVertex!");

			m_vertexBuffer = nullptr;
			m_indexBuffer = nullptr;

			if (!vertices.empty())
			{
				auto vertexStaging = Buffer(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertices.data());
				m_vertexBuffer = std::make_unique<Buffer>(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				m_vertexCount = vertices.size();

				CommandBuffer commandBuffer = CommandBuffer();

				VkBufferCopy copyRegion = {};
				copyRegion.size = sizeof(T) * vertices.size();
				vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), vertexStaging.GetBuffer(), m_vertexBuffer->GetBuffer(), 1, &copyRegion);

				commandBuffer.End();
				commandBuffer.SubmitIdle();
			}

			if (!indices.empty())
			{
				auto indexStaging = Buffer(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices.data());
				m_indexBuffer = std::make_unique<Buffer>(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				m_indexCount = indices.size();

				CommandBuffer commandBuffer = CommandBuffer();

				VkBufferCopy copyRegion = {};
				copyRegion.size = sizeof(uint32_t) * indices.size();
				vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), indexStaging.GetBuffer(), m_indexBuffer->GetBuffer(), 1, &copyRegion);

				commandBuffer.End();
				commandBuffer.SubmitIdle();
			}

			m_minExtents = Vector3::PositiveInfinity;
			m_maxExtents = Vector3::NegativeInfinity;

			for (const auto &vertex : vertices)
			{
				Vector3 position = vertex.GetPosition();
				m_minExtents = Vector3::MinVector(m_minExtents, position);
				m_maxExtents = Vector3::MinVector(m_maxExtents, position);
			}

			// FIXME: Radius calculation might be wrong.
			float min0 = std::abs(m_minExtents.MaxComponent());
			float min1 = std::abs(m_minExtents.MinComponent());
			float max0 = std::abs(m_maxExtents.MaxComponent());
			float max1 = std::abs(m_maxExtents.MinComponent());
			m_radius = std::max(min0, std::max(min1, std::max(max0, max1)));
		}
	private:
		std::unique_ptr<Buffer> m_vertexBuffer;
		std::unique_ptr<Buffer> m_indexBuffer;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;

		Vector3 m_minExtents;
		Vector3 m_maxExtents;
		float m_radius;
	};
}
