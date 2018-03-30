#include "FilterFxaa.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	FilterFxaa::FilterFxaa(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Fxaa.frag", graphicsStage, { }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_spanMax(8.0f)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
		delete m_uniformScene;
	}

	void FilterFxaa::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_uniformScene,
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(2)
		});

		// Updates uniforms.
		UboScene uboScene = {};
		uboScene.spanMax = m_spanMax;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
