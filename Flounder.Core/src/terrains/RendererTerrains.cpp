#include "RendererTerrains.hpp"

#include "ShaderTerrains.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	RendererTerrains::RendererTerrains() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(ShaderTerrains::UboScene))),
		m_shader(new Shader("terrains", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/terrains/terrain.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/terrains/terrain.frag.spv")
		})),
		m_pipeline(new Pipeline("waters", m_shader, ShaderTerrains::pipelineCreateInfo, ShaderTerrains::inputState, ShaderTerrains::descriptor))
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_shader;
	}

	void RendererTerrains::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		ShaderTerrains::UboScene uboScene = {};
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
