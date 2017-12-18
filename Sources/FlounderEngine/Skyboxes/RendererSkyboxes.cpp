#include "RendererSkyboxes.hpp"

#include "UbosSkyboxes.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
	{
		PIPELINE_MRT_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_FRONT_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
			UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL), // uboObject
			Cubemap::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerCubemap
		}, // descriptors

		{ "Resources/Shaders/Skyboxes/Skybox.vert.spv", "Resources/Shaders/Skyboxes/Skybox.frag.spv" } // shaderStages
	};

	RendererSkyboxes::RendererSkyboxes(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosSkyboxes::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosSkyboxes::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		Skyboxes::Get()->GetSkybox()->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
	}
}
