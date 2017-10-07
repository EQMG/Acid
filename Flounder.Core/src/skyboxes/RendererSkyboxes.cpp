#include "RendererSkyboxes.hpp"

#include "SkyboxesShader.hpp"

namespace Flounder
{
	RendererSkyboxes::RendererSkyboxes() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(SkyboxesShader::UboScene))),

		m_shader(new Shader("skyboxes", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/skyboxes/skybox.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/skyboxes/skybox.frag.spv")
		})),
		m_pipeline(new Pipeline("skyboxes", m_shader, SkyboxesShader::pipelineCreateInfo, SkyboxesShader::inputState, SkyboxesShader::descriptor))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_uniformScene;

		delete m_shader;
		delete m_pipeline;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		SkyboxesShader::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		Skyboxes::Get()->GetSkybox()->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
	}
}
