#include "FilterDof.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterDof::FilterDof(const GraphicsStage &graphicsStage, const float &maxBlurSize, const float &radiusScale, const float &goldenAngle) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Dof.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_maxBlurSize(maxBlurSize),
		m_radiusScale(radiusScale),
		m_goldenAngle(goldenAngle)
	{
	}

	void FilterDof::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("maxBlurSize", m_maxBlurSize);
		m_uniformScene.Push("radiusScale", m_radiusScale);
		m_uniformScene.Push("goldenAngle", m_goldenAngle);
		m_uniformScene.Push("nearPlane", camera.GetNearPlane());
		m_uniformScene.Push("farPlane", camera.GetFarPlane());

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerDepth", Renderer::Get()->GetAttachment("depth"));
		m_descriptorSet.Push("samplerColour", Renderer::Get()->GetAttachment("resolved"));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
