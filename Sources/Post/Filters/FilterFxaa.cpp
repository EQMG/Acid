#include "FilterFxaa.hpp"

namespace fl
{
	FilterFxaa::FilterFxaa(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Fxaa.frag", graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
		m_spanMax(8.0f)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
		delete m_uniformScene;
	}

	void FilterFxaa::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_uniformScene->Push("spanMax", m_spanMax);

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
