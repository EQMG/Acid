#pragma once

#include <cmath>
#include <string>
#include <vector>
#include "Maths/Vector3.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
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

		bool CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances = 1);

		virtual void Load() override;

		virtual void Decode(const Metadata &metadata) override;

		virtual void Encode(Metadata &metadata) const override;

		std::vector<float> GetPointCloud() const;

		const Vector3 &GetMinExtents() const { return m_minExtents; }

		const Vector3 &GetMaxExtents() const { return m_maxExtents; }

		float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

		float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

		float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

		float GetRadius() const { return m_radius; }

		const VertexBuffer *GetVertexBuffer() const { return m_vertexBuffer.get(); }

		const IndexBuffer *GetIndexBuffer() const { return m_indexBuffer.get(); }
	protected:
		template<typename T>
		void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {})
		{
			static_assert(std::is_base_of<IVertex, T>::value, "T must derive from IVertex!");

			if (!vertices.empty())
			{
				m_vertexBuffer = std::make_unique<VertexBuffer>(sizeof(T), vertices.size(), vertices.data());
			}

			if (!indices.empty())
			{
				m_indexBuffer = std::make_unique<IndexBuffer>(VK_INDEX_TYPE_UINT32, sizeof(uint32_t), indices.size(), indices.data());
			}

			m_minExtents = Vector3::PositiveInfinity;
			m_maxExtents = Vector3::NegativeInfinity;

			for (const auto &vertex : vertices)
			{
				Vector3 position = vertex.GetPosition();
				m_minExtents = Vector3::MinVector(m_minExtents, position);
				m_maxExtents = Vector3::MinVector(m_maxExtents, position);
			}

			float min0 = std::abs(m_minExtents.MaxComponent());
			float min1 = std::abs(m_minExtents.MinComponent());
			float max0 = std::abs(m_maxExtents.MaxComponent());
			float max1 = std::abs(m_maxExtents.MinComponent());
			m_radius = std::max(min0, std::max(min1, std::max(max0, max1)));
		}
	private:
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;

		Vector3 m_minExtents;
		Vector3 m_maxExtents;
		float m_radius;
	};
}
