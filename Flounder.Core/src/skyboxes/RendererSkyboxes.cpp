#include "RendererSkyboxes.hpp"

#include "UbosSkyboxes.hpp"

namespace Flounder
{
	const DescriptorType RendererSkyboxes::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererSkyboxes::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererSkyboxes::typeSamplerCubemap = Cubemap::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererSkyboxes::pipelineCreateInfo =
	{
		PIPELINE_MRT_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_FRONT_BIT, // cullModeFlags
		1, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerCubemap }, // descriptors

		{ "res/shaders/skyboxes/skybox.vert.spv", "res/shaders/skyboxes/skybox.frag.spv" } // shaderStages
	};

	RendererSkyboxes::RendererSkyboxes() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosSkyboxes::UboScene))),
		m_pipeline(new Pipeline("skyboxes", pipelineCreateInfo))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosSkyboxes::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		Skyboxes::Get()->GetSkybox()->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
	}
}
