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
}
