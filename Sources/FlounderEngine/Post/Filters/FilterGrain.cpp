#include "FilterGrain.hpp"

namespace Flounder
{
	FilterGrain::FilterGrain(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Grain.frag", graphicsStage, {}),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_strength(2.3f)
	{
	}

	FilterGrain::~FilterGrain()
	{
		delete m_uniformScene;
	}

	void FilterGrain::Render(const VkCommandBuffer &commandBuffer)
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
		uboScene.strength = m_strength;
		m_uniformScene->Update(&uboScene);


		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
