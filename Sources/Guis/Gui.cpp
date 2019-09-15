#include "Gui.hpp"

#include "Graphics/Graphics.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Maths/Visual/DriverConstant.hpp"

namespace acid
{
Gui::Gui(UiObject *parent, const UiTransform &rectangle, std::shared_ptr<Image2d> image, const Colour &colourOffset) :
	UiObject(parent, rectangle),
	m_model(ModelRectangle::Create(0.0f, 1.0f)),
	m_image(std::move(image)),
	m_atlasScale(1.0f),
	m_colourDriver(std::make_unique<DriverConstant<Colour>>(colourOffset))
{
}

void Gui::UpdateObject()
{
	auto numberOfRows = m_image ? m_numberOfRows : 1;
	auto column = m_selectedRow % numberOfRows;
	auto row = m_selectedRow / numberOfRows;
	m_atlasOffset = Vector2f(static_cast<float>(column), static_cast<float>(row)) / static_cast<float>(numberOfRows);

	m_colourOffset = m_colourDriver->Update(Engine::Get()->GetDelta());

	// Updates uniforms.
	m_uniformObject.Push("modelView", GetModelView());
	m_uniformObject.Push("alpha", GetScreenAlpha());

	m_uniformObject.Push("aspectRatio", (GetScreenTransform().GetSize().m_x / GetScreenTransform().GetSize().m_y));

	m_uniformObject.Push("colourOffset", m_colourOffset);
	m_uniformObject.Push("atlasOffset", m_atlasOffset);
	m_uniformObject.Push("atlasScale", m_atlasScale);
	m_uniformObject.Push("atlasRows", static_cast<float>(m_numberOfRows));
	m_uniformObject.Push("ninePatches", m_ninePatches);
}

bool Gui::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline)
{
	// Gets if this should be rendered.
	if (!m_image || !IsEnabled())
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	m_descriptorSet.Push("samplerColour", m_image);

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	auto scissor = GetScissor();
	VkRect2D scissorRect = {};
	scissorRect.offset.x = scissor ? static_cast<int32_t>(scissor->m_x) : 0;
	scissorRect.offset.y = scissor ? static_cast<int32_t>(scissor->m_y) : 0;
	scissorRect.extent.width = scissor ? static_cast<int32_t>(scissor->m_z) : Window::Get()->GetSize().m_x;
	scissorRect.extent.height = scissor ? static_cast<int32_t>(scissor->m_w) : Window::Get()->GetSize().m_y;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return m_model->CmdRender(commandBuffer);
}
}
