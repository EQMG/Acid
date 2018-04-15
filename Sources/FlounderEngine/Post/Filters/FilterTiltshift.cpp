#include "FilterTiltshift.hpp"

namespace Flounder
{
	FilterTiltshift::FilterTiltshift(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Tiltshift.frag", graphicsStage, {}),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_blurAmount(1.0f),
		m_centre(1.1f),
		m_stepSize(0.004f),
		m_steps(3.0f)
	{
	}

	FilterTiltshift::~FilterTiltshift()
	{
		delete m_uniformScene;
	}

	void FilterTiltshift::Render(const VkCommandBuffer &commandBuffer)
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
		uboScene.blurAmount = m_blurAmount;
		uboScene.centre = m_centre;
		uboScene.stepSize = m_stepSize;
		uboScene.steps = m_steps;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
