#include "FilterGrain.hpp"

namespace fl
{
	FilterGrain::FilterGrain(const GraphicsStage &graphicsStage) :
		IPostFilter({"Resources/Shaders/Filters/Default.vert", "Resources/Shaders/Filters/Grain.frag"}, graphicsStage, {}),
		m_uniformScene(UniformHandler()),
		m_strength(2.3f)
	{
	}

	FilterGrain::~FilterGrain()
	{
	}

	void FilterGrain::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_uniformScene.Push("strength", m_strength);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
		bool descriptorsSet = m_descriptorSet.Update(m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
