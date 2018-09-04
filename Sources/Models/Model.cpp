#include "Model.hpp"

#include <cassert>
#include <cmath>

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.obj";

	Model::Model() :
		IResource(),
		m_filename(""),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_pointCloud(std::vector<float>()),
		m_minExtents(Vector3()),
		m_maxExtents(Vector3())
	{
	}

	Model::Model(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_pointCloud(std::vector<float>()),
		m_minExtents(Vector3()),
		m_maxExtents(Vector3())
	{
		void *verticesData = vertices[0]->GetData(vertices);
		m_vertexBuffer = std::make_shared<VertexBuffer>(vertices[0]->GetSize(), vertices.size(), verticesData);
		free(verticesData);

		m_indexBuffer = std::make_shared<IndexBuffer>(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());

		CalculateBounds(vertices);

		for (auto &vertex : vertices)
		{
			delete vertex;
		}
	}

	Model::Model(std::vector<IVertex *> &vertices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_pointCloud(std::vector<float>()),
		m_minExtents(Vector3()),
		m_maxExtents(Vector3())
	{
		void *verticesData = vertices[0]->GetData(vertices);
		m_vertexBuffer = std::make_shared<VertexBuffer>(vertices[0]->GetSize(), vertices.size(), verticesData);
		free(verticesData);

		CalculateBounds(vertices);

		for (auto &vertex : vertices)
		{
			delete vertex;
		}
	}

	Model::~Model()
	{
	}

	void Model::CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances)
	{
		if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
		{
			VkBuffer vertexBuffers[] = {m_vertexBuffer->GetBuffer()};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer.GetCommandBuffer(), m_indexBuffer->GetBuffer(), 0, m_indexBuffer->GetIndexType());
			vkCmdDrawIndexed(commandBuffer.GetCommandBuffer(), m_indexBuffer->GetIndexCount(), instances, 0, 0, 0);
		}
		else if (m_vertexBuffer != nullptr && m_indexBuffer == nullptr)
		{
			VkBuffer vertexBuffers[] = {m_vertexBuffer->GetBuffer()};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, vertexBuffers, offsets);
			vkCmdDraw(commandBuffer.GetCommandBuffer(), m_vertexBuffer->GetVertexCount(), instances, 0, 0);
		}
		else
		{
			assert(false && "Cannot render model, no buffers exist for it!");
		}
	}

	float Model::GetRadius() const
	{
		float min0 = std::abs(m_minExtents.MaxComponent());
		float min1 = std::abs(m_minExtents.MinComponent());
		float max0 = std::abs(m_maxExtents.MaxComponent());
		float max1 = std::abs(m_maxExtents.MinComponent());
		return std::max(min0, std::max(min1, std::max(max0, max1)));
	}

	void Model::Set(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name)
	{
		m_filename = name;

		if (!vertices.empty())
		{
			void *verticesData = vertices[0]->GetData(vertices);
			m_vertexBuffer = std::make_shared<VertexBuffer>(vertices[0]->GetSize(), vertices.size(), verticesData);
			free(verticesData);
		}

		if (!indices.empty())
		{
			m_indexBuffer = std::make_shared<IndexBuffer>(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());
		}

		CalculateBounds(vertices);

		for (auto &vertex : vertices)
		{
			delete vertex;
		}
	}

	void Model::CalculateBounds(const std::vector<IVertex *> &vertices)
	{
		m_pointCloud.clear();
		m_pointCloud.resize(vertices.size() * 3);

		m_minExtents.m_x = +std::numeric_limits<float>::infinity();
		m_minExtents.m_y = +std::numeric_limits<float>::infinity();
		m_minExtents.m_z = +std::numeric_limits<float>::infinity();
		m_maxExtents.m_x = -std::numeric_limits<float>::infinity();
		m_maxExtents.m_y = -std::numeric_limits<float>::infinity();
		m_maxExtents.m_z = -std::numeric_limits<float>::infinity();

		int i = 0;

		for (auto &vertex : vertices)
		{
			Vector3 position = vertex->GetPosition();

			m_pointCloud[i] = position.m_x;
			m_pointCloud[i + 1] = position.m_y;
			m_pointCloud[i + 2] = position.m_z;
			i += 3;

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
		}
	}
}
