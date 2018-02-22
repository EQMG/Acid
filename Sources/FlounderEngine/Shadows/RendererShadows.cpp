#include <Meshes/Mesh.hpp>
#include "RendererShadows.hpp"

#include "../Devices/Display.hpp"
#include "../Materials/Material.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosShadows.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_FRONT_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(0), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT) // uboObject
			}, // descriptors

			{"Resources/Shaders/Shadows/Shadow.vert.spv", "Resources/Shaders/Shadows/Shadow.frag.spv"} // shaderStages
		};

	RendererShadows::RendererShadows(const int &subpass) :
		IRenderer(),
		m_uniformObject(new UniformBuffer(sizeof(UbosShadows::UboObject))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_pipeline;
	}

	void RendererShadows::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

		for (auto object : *Scenes::Get()->GetStructure()->GetAll())
		{
			auto mesh = object->GetComponent<Mesh>();

			if (mesh == nullptr || mesh->GetModel() == nullptr)
			{
				continue;
			}

			auto material = object->GetComponent<Material>();

			if (material == nullptr || !material->GetCastsShadows())
			{
				continue;
			}

			Matrix4 modelMatrix = Matrix4();
			object->GetTransform()->GetWorldMatrix(&modelMatrix);
			RenderModel(commandBuffer, mesh->GetModel(), modelMatrix);
		}
	}

	void RendererShadows::RenderModel(const VkCommandBuffer &commandBuffer, Model *object, const Matrix4 &modelMatrix)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		UbosShadows::UboObject uboObject = {};
		uboObject.mvp = Matrix4();
		Matrix4::Multiply(*Shadows::Get()->GetShadowBox()->GetProjectionViewMatrix(), modelMatrix, &uboObject.mvp);
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			m_uniformObject->GetWriteDescriptor(0, descriptorSet)
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		object->CmdRender(commandBuffer);
	}
}
