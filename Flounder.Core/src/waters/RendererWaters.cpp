#include "RendererWaters.hpp"

#include "ShaderWaters.hpp"

namespace Flounder
{
	RendererWaters::RendererWaters() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(ShaderWaters::UboScene))),
		m_shader(new Shader("waters", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/waters/water.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/waters/water.frag.spv")
		})),
		m_pipeline(new Pipeline("waters", m_shader, ShaderWaters::pipelineCreateInfo, ShaderWaters::inputState, ShaderWaters::descriptor))
	{
	}

	RendererWaters::~RendererWaters()
	{
		delete m_uniformScene;
		delete m_shader;
		delete m_pipeline;
	}

	void RendererWaters::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		ShaderWaters::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		Waters::Get()->GetWater()->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
	}
}
