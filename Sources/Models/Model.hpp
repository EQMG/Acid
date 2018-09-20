#pragma once

#include <cmath>
#include <string>
#include <vector>
#include "Maths/Vector3.hpp"
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Resources/IResource.hpp"
#include "IVertex.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class ACID_EXPORT Model :
		public IResource
	{
	private:
		std::string m_filename;

		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;

		std::vector<float> m_pointCloud;

		Vector3 m_minExtents;
		Vector3 m_maxExtents;
		float m_radius;
	public:
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
		explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}, const std::string &name = "") :
			Model()
		{
			Initialize(vertices, indices, name);
		}

		bool CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances = 1);

		std::string GetFilename() override { return m_filename; }

		Vector3 GetMinExtents() const { return m_minExtents; }

		Vector3 GetMaxExtents() const { return m_maxExtents; }

		std::vector<float> GetPointCloud() const { return m_pointCloud; }

		float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

		float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

		float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

		float GetRadius() const { return m_radius; }

		VertexBuffer *GetVertexBuffer() const { return m_vertexBuffer.get(); }

		IndexBuffer *GetIndexBuffer() const { return m_indexBuffer.get(); }

	protected:
		template<typename T>
		void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}, const std::string &name = "")
		{
			static_assert(std::is_base_of<IVertex, T>::value, "T must derive from IVertex!");

			m_filename = name;

			if (!vertices.empty())
			{
				m_vertexBuffer = std::make_unique<VertexBuffer>(sizeof(T), vertices.size(), vertices.data());
			}

			if (!indices.empty())
			{
				m_indexBuffer = std::make_unique<IndexBuffer>(VK_INDEX_TYPE_UINT32, sizeof(uint32_t), indices.size(), indices.data());
			}

			m_pointCloud = std::vector<float>(3 * vertices.size());
			m_minExtents = Vector3::POSITIVE_INFINITY;
			m_maxExtents = Vector3::NEGATIVE_INFINITY;

			uint32_t i = 0;

			for (auto &vertex : vertices)
			{
				Vector3 position = vertex.GetPosition();
				m_pointCloud[i * 3] = position.m_x;
				m_pointCloud[i * 3 + 1] = position.m_z;
				m_pointCloud[i * 3 + 2] = position.m_y;

				if (position.m_x < m_minExtents.m_x)
				{
					m_minExtents.m_x = position.m_x;
				}
				else if (position.m_x > m_maxExtents.m_x)
				{
					m_maxExtents.m_x = position.m_x;
				}

				if (position.m_y < m_minExtents.m_y)
				{
					m_minExtents.m_y = position.m_y;
				}
				else if (position.m_y > m_maxExtents.m_y)
				{
					m_maxExtents.m_y = position.m_y;
				}

				if (position.m_z < m_minExtents.m_z)
				{
					m_minExtents.m_z = position.m_z;
				}
				else if (position.m_z > m_maxExtents.m_z)
				{
					m_maxExtents.m_z = position.m_z;
				}

				i++;
			}

			float min0 = std::abs(m_minExtents.MaxComponent());
			float min1 = std::abs(m_minExtents.MinComponent());
			float max0 = std::abs(m_maxExtents.MaxComponent());
			float max1 = std::abs(m_maxExtents.MinComponent());
			m_radius = std::max(min0, std::max(min1, std::max(max0, max1)));
		}
	};
}
