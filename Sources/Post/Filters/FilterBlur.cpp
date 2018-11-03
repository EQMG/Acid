#include "FilterBlur.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterBlur::FilterBlur(const GraphicsStage &graphicsStage, const Vector2 &direction, const BlurType &blurType) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Blur.frag"}, {PipelineDefine("BLUR_TYPE", String::To(blurType))}),
		m_uniformScene(UniformHandler()),
		m_blurType(blurType),
		m_direction(direction)
	{
	}

	void FilterBlur::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("direction", m_direction);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerColour", Renderer::Get()->GetAttachment("resolved"));
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
