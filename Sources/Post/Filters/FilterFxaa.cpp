#include "FilterFxaa.hpp"

namespace acid
{
	FilterFxaa::FilterFxaa(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Fxaa.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_spanMax(8.0f)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
	}

	void FilterFxaa::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("spanMax", m_spanMax);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeAlbedo", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerAlbedo", m_pipeline.GetTexture(3));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
