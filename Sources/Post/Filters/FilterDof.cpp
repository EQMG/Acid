#include "FilterDof.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterDof::FilterDof(const GraphicsStage &graphicsStage, PipelineBlur *pipelineBlur, const float &focusPoint, const float &nearField,
	                     const float &nearTransition, const float &farField, const float &farTransition) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Dof.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_pipelineBlur(pipelineBlur),
		m_focusPoint(focusPoint),
		m_nearField(nearField),
		m_nearTransition(nearTransition),
		m_farField(farField),
		m_farTransition(farTransition)
	{
	}

	void FilterDof::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("nearPlane", camera.GetNearPlane());
		m_uniformScene.Push("farPlane", camera.GetFarPlane());
		m_uniformScene.Push("focusPoint", m_focusPoint);
		m_uniformScene.Push("nearField", m_nearField);
		m_uniformScene.Push("nearTransition", m_nearTransition);
		m_uniformScene.Push("farField", m_farField);
		m_uniformScene.Push("farTransition", m_farTransition);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
	//	m_descriptorSet.Push("writeColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerDepth", Renderer::Get()->GetAttachment("depth"));
	//	m_descriptorSet.Push("samplerColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerBlured", m_pipelineBlur == nullptr ? nullptr : m_pipelineBlur->GetOutput());
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
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
