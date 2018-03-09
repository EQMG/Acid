#include "RendererShadows.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Entities/EntityRender.hpp"
#include "../Terrains/TerrainRender.hpp"
#include "../Materials/Material.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosShadows.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_FRONT_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(0), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL) // uboObject
			}, // descriptors

			{"Resources/Shaders/Shadows/Shadow.vert.spv", "Resources/Shaders/Shadows/Shadow.frag.spv"} // shaderStages
		};

	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosShadows::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PIPELINE_CREATE))
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererShadows::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosShadows::UboScene uboScene = {};
		uboScene.projectionView = *Shadows::Get()->GetShadowBox()->GetProjectionViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<Mesh *> meshList = std::vector<Mesh *>();
		Scenes::Get()->GetStructure()->QueryComponents(&meshList);

		for (auto mesh : meshList)
		{
			// Gets required components.
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
		UniformBuffer *uniformObject;
		DescriptorSet *descriptorSet;

		auto entityRender = gameObject->GetComponent<EntityRender>();
		auto terrainRender = gameObject->GetComponent<TerrainRender>();

		if (entityRender != nullptr)
		{
			uniformObject = entityRender->GetUniformObject();
		//	descriptorSet = entityRender->GetShadowDescriptorSet();
		}
		else if (terrainRender != nullptr)
		{
			uniformObject = terrainRender->GetUniformObject();
		//	descriptorSet = terrainRender->GetShadowDescriptorSet();
		}
		else
		{
			return;
		}

		// Updates descriptors.
		if (descriptorSet == nullptr)
		{
			descriptorSet = new DescriptorSet(*m_pipeline);
		}

		descriptorSet->Update({
			m_uniformScene,
			uniformObject
		});

		// Draws the object.
		descriptorSet->BindDescriptor(commandBuffer);
		object->CmdRender(commandBuffer);
	}
}
