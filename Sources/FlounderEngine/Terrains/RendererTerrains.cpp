#include "RendererTerrains.hpp"

#include "../Objects/Objects.hpp"
#include "../Models/Model.hpp"
#include "Terrains.hpp"
#include "UbosTerrains.hpp"

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
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL) // uboObject
			}, // descriptors

			{
				"Resources/Shaders/Terrains/Terrain.vert.spv", "Resources/Shaders/Terrains/Terrain.frag.spv"
			} // shaderStages
		};

	RendererTerrains::RendererTerrains(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTerrains::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererTerrains::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosTerrains::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		for (auto object : *Objects::Get()->GetStructure()->GetAll())
		{
			auto entityRender = object->GetComponent<TerrainRender>();

			if (entityRender != nullptr)
			{
				entityRender->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
			}
		}
	}
}
