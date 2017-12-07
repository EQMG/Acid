#include "RendererTerrains.hpp"

#include "Terrains.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	const DescriptorType RendererTerrains::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTerrains::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const PipelineCreateInfo RendererTerrains::pipelineCreateInfo =
	{
		PIPELINE_MRT, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject }, // descriptors

		{ "Resources/Shaders/Terrains/Terrain.vert.spv", "Resources/Shaders/Terrains/Terrain.frag.spv" } // shaderStages
	};

	RendererTerrains::RendererTerrains(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTerrains::UboScene))),
		m_pipeline(new Pipeline(pipelineCreateInfo, subpass))
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererTerrains::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosTerrains::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		for (auto object : *Terrains::Get()->GetTerrains())
		{
			object->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
