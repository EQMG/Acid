#include "Model.hpp"

#include <cassert>
#include "Maths/Maths.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	std::shared_ptr<Model> Model::Create(const std::string &data)
	{
		return Scenes::Get()->GetModelRegister().Create(data);
	}

	Model::Model() :
		Resource(""),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_minExtents(Vector3()),
		m_maxExtents(Vector3()),
		m_radius(0.0f)
	{
	}

	bool Model::CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances)
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
			return false;
		}

		return true;
	}

	void Model::Encode(Metadata &metadata) const
	{
	}

	std::vector<float> Model::GetPointCloud() const
	{
		if (m_vertexBuffer == nullptr)
		{
			return std::vector<float>();
		}

		std::vector<float> result(3 * m_vertexBuffer->GetSize());
		m_vertexBuffer->CopyBuffer(result.data());
		return result;
	}
}
