#include "Model.hpp"

#include <cassert>
#include "Helpers/FileSystem.hpp"
#include "Obj/ModelObj.hpp"
#include "Shapes/ModelCube.hpp"
#include "Shapes/ModelCylinder.hpp"
#include "Shapes/ModelDisk.hpp"
#include "Shapes/ModelRectangle.hpp"
#include "Shapes/ModelSphere.hpp"

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.obj";

	std::shared_ptr<Model> Model::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		std::string fileExt = String::Lowercase(FileSystem::FileSuffix(data));

		if (fileExt == ".obj")
		{
			return ModelObj::Resource(data);
		}

		// TODO: Modularize.
		auto split = String::Split(data, "_");

		if (!split.empty() && split[0] == "Cube")
		{
			return ModelCube::Resource(data);
		}

		if (!split.empty() && split[0] == "Cylinder")
		{
			return ModelCylinder::Resource(data);
		}

		if (!split.empty() && split[0] == "Disk")
		{
			return ModelDisk::Resource(data);
		}

		if (!split.empty() && split[0] == "Rectangle")
		{
			return ModelRectangle::Resource(data);
		}

		if (!split.empty() && split[0] == "Sphere")
		{
			return ModelSphere::Resource(data);
		}

		return nullptr;
	}

	Model::Model() :
		IResource(),
		m_filename(""),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_pointCloud(std::vector<float>()),
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
}
