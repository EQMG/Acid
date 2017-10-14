#include "RendererTerrains.hpp"

#include "Terrains.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	const DescriptorType RendererTerrains::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTerrains::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const PipelineCreateInfo RendererTerrains::pipelineCreateInfo =
	{
		PIPELINE_POLYGON, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags
		InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions()), // inputState
		Descriptor::Create({ typeUboScene, typeUboObject }), // descriptor
		ShaderStages::Create({ ShaderStage("res/shaders/terrains/terrain.vert.spv", VK_SHADER_STAGE_VERTEX_BIT), ShaderStage("res/shaders/terrains/terrain.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT) }) // shaderStages
	};

	RendererTerrains::RendererTerrains() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTerrains::UboScene))),
		m_pipeline(new Pipeline("terrains", pipelineCreateInfo))
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

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto object : *Terrains::Get()->GetTerrains())
		{
			object->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
