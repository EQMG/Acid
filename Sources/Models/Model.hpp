#pragma once

#include <string>
#include <vector>
#include "Renderer/Buffers/IndexBuffer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Resources/Resources.hpp"
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

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

		std::vector<float> m_pointCloud;

		Vector3 m_minExtents;
		Vector3 m_maxExtents;
	public:
		/// <summary>
		/// Creates a new empty model.
		/// </summary>
		Model();

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="indices"> The model indices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

		/// <summary>
		/// Creates a new model without indices.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<IVertex *> &vertices, const std::string &name = "");

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~Model();

		void CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances = 1);

		std::string GetFilename() override { return m_filename; }

		Vector3 GetMinExtents() const { return m_minExtents; }

		Vector3 GetMaxExtents() const { return m_maxExtents; }

		std::vector<float> GetPointCloud() const { return m_pointCloud; }

		float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

		float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

		float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

		float GetRadius() const;

		std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_vertexBuffer; }

		std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_indexBuffer; }

	protected:
		void Set(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

	private:
		void CalculateBounds(const std::vector<IVertex *> &vertices);
	};
}
