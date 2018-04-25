#include "FilterTiltshift.hpp"

namespace fl
{
	FilterTiltshift::FilterTiltshift(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Tiltshift.frag", graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
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
		// Updates uniforms.
		m_uniformScene->Push("blurAmount", m_blurAmount);
		m_uniformScene->Push("centre", m_centre);
		m_uniformScene->Push("stepSize", m_stepSize);
		m_uniformScene->Push("steps", m_steps);

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", m_uniformScene);
		m_descriptorSet->Push("writeColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerColour", m_pipeline->GetTexture(2));
		bool descriptorsSet = m_descriptorSet->Update(*m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
