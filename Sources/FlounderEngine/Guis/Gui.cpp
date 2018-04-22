#include "Gui.hpp"

#include "Models/Shapes/ShapeRectangle.hpp"

namespace Flounder
{
	Gui::Gui(UiObject *parent, const UiBound &rectangle, Texture *texture, const int &selectedRow) :
		UiObject(parent, rectangle),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler()),
		m_model(ShapeRectangle::Resource(0.0f, 1.0f)),
		m_texture(texture),
		m_selectedRow(selectedRow),
		m_atlasOffset(new Vector2()),
		m_colourOffset(new Colour(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	Gui::~Gui()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
		delete m_model;
		delete m_atlasOffset;
		delete m_colourOffset;
	}

	void Gui::UpdateObject()
	{
		const int numberOfRows = m_texture != nullptr ? m_texture->GetNumberOfRows() : 1;
		const int column = m_selectedRow % numberOfRows;
		const int row = m_selectedRow / numberOfRows;
		*m_atlasOffset = Vector2(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));

		// Updates uniforms.
		m_uniformObject->Push("transform", *GetScreenTransform());
		m_uniformObject->Push("colourOffset", *m_colourOffset);
		m_uniformObject->Push("atlasOffset", *m_atlasOffset);
		m_uniformObject->Push("atlasRows", static_cast<float>(m_texture->GetNumberOfRows()));
		m_uniformObject->Push("alpha", GetAlpha());
	}

	void Gui::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		// Gets if this should be rendered.
		if (!IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet->Push("UboObject", m_uniformObject);
		m_descriptorSet->Push("samplerColour", m_texture);
		bool descriptorsSet = m_descriptorSet->Update(pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_x);
		scissorRect.offset.y = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_y);
		scissorRect.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_z);
		scissorRect.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_w);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
