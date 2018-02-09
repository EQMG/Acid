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
			Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

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
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererEntities::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosEntities::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		/*auto mapPrefabs = std::map<std::string, std::vector<Entity *>>();

		for (auto entity : *Entities::Get()->GetStructure()->GetAll())
		{
			if (entity->GetCollider() != nullptr && !entity->GetCollider()->InFrustum(*camera.GetViewFrustum()))
			{
				continue;
			}

			auto key = mapPrefabs.find(entity->GetPrefabName());

			if (key == mapPrefabs.end())
			{
				mapPrefabs.insert(std::make_pair(entity->GetPrefabName(), std::vector<Entity *>()));
				key = mapPrefabs.find(entity->GetPrefabName());
			}

			(*key).second.push_back(entity);
		}

		for (auto prefab : mapPrefabs)
		{
		//	printf("%s: %i\n", prefab.first.c_str(), prefab.second.size());
		}*/

		//	std::vector<Entity*> inFrustum = std::vector<Entity*>();
		//	Entities::Get()->GetStructure()->QueryFrustum(camera.GetViewFrustum(), &inFrustum);

		for (auto entity : *Entities::Get()->GetStructure()->GetAll()) // inFrustum
		{
			entity->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
