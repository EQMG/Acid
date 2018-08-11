#include "FilterDefault.hpp"

namespace acid
{
	FilterDefault::FilterDefault(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Default.frag"}, {})
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates descriptors.
		m_descriptorSet.Push("writeAlbedo", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerAlbedo", m_pipeline.GetTexture(3));
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
