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

		std::vector<Mesh *> meshList = std::vector<Mesh *>();
		Scenes::Get()->GetStructure()->QueryComponents(&meshList);

		for (auto mesh : meshList)
		{
			if (mesh->GetModel() == nullptr)
			{
				continue;
			}

			auto material = mesh->GetGameObject()->GetComponent<Material>();

			if (material == nullptr || !material->GetCastsShadows())
			{
				continue;
			}

			RenderModel(commandBuffer, mesh->GetModel(), mesh->GetGameObject());
		}
	}

	void RendererShadows::RenderModel(const VkCommandBuffer &commandBuffer, Model *object, GameObject *gameObject)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		Matrix4 worldMatrix = Matrix4();
		gameObject->GetTransform()->GetWorldMatrix(&worldMatrix);

		// Creates a UBO object and write descriptor.
		UbosShadows::UboObject uboObject = {};
		uboObject.mvp = (*Scenes::Get()->GetCamera()->GetProjectionMatrix() * *Scenes::Get()->GetCamera()->GetViewMatrix()) * worldMatrix;
		//	Matrix4::Multiply(*Shadows::Get()->GetShadowBox()->GetProjectionViewMatrix(), modelMatrix, &uboObject.mvp);
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			m_uniformObject->GetWriteDescriptor(0, descriptorSet)
		};

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		// Draws the object.
		VkDescriptorSet descriptors[] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		object->CmdRender(commandBuffer);
	}
}
