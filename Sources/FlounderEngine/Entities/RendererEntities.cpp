#include "RendererEntities.hpp"

#include "../Renderer/Renderer.hpp"
#include "Entities.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
		{
			PIPELINE_MRT, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(2), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL), // uboObject
				Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerDiffuse
				Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT) // samplerSway
			}, // descriptors

			{"Resources/Shaders/Entities/Entity.vert.spv", "Resources/Shaders/Entities/Entity.frag.spv"} // shaderStages
		};

	RendererEntities::RendererEntities(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosEntities::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO)),
		m_instanceBuffers(new std::map<Model *, EntityInstance *>())
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
		delete m_pipeline;

		for (const auto &instance : *m_instanceBuffers)
		{
			delete instance.second;
		}

		delete m_instanceBuffers;
	}

	void RendererEntities::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosEntities::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		/*for (const auto &instance : *m_instanceBuffers)
		{
			instance.second->objects.clear();
		}

		for (auto entity : *Entities::Get()->GetStructure()->GetAll())
		{
			auto componentModel = entity->GetComponent<ComponentModel>();

			if (componentModel == nullptr || componentModel->GetModel() == nullptr)
			{
				continue;
			}

			if (entity->GetCollider() != nullptr && !entity->GetCollider()->InFrustum(*camera.GetViewFrustum()))
			{
				continue;
			}

			auto key = m_instanceBuffers->find(componentModel->GetModel());

			if (key == m_instanceBuffers->end())
			{
				m_instanceBuffers->insert(std::make_pair(componentModel->GetModel(), new EntityInstance()));
				key = m_instanceBuffers->find(componentModel->GetModel());
			}

			(*key).second->objects.push_back(entity->GetUboObject());
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		for (auto instance : *m_instanceBuffers)
		{
			instance.second->uniformBuffer->Update(&instance.second->objects);

			std::vector<VkWriteDescriptorSet> descriptorWrites = {m_uniformScene->GetWriteDescriptor(0, descriptorSet), instance.second->uniformBuffer->GetWriteDescriptor(1, descriptorSet)};
			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

			VkDescriptorSet descriptors[] = {descriptorSet};
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

			instance.first->CmdRender(commandBuffer, instance.second->objects.size());
		}*/

		//	std::vector<Entity*> inFrustum = std::vector<Entity*>();
		//	Entities::Get()->GetStructure()->QueryFrustum(camera.GetViewFrustum(), &inFrustum);

		for (auto entity : *Entities::Get()->GetStructure()->GetAll()) // inFrustum
		{
			entity->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
