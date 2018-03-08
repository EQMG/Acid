#include "Gui.hpp"

#include "../Devices/Display.hpp"
#include "../Models/Shapes/ShapeRectangle.hpp"
#include "UbosGuis.hpp"

namespace Flounder
{
	Gui::Gui(UiObject *parent, const UiBound &rectangle, Texture *texture, const int &selectedRow) :
		UiObject(parent, rectangle),
		m_uniformObject(new UniformBuffer(sizeof(UbosGuis::UboObject))),
		m_model(ShapeRectangle::Resource(0.0f, 1.0f)),
		m_texture(texture),
		m_selectedRow(selectedRow),
		m_atlasOffset(new Vector2()),
		m_colourOffset(new Colour(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	Gui::~Gui()
	{
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
		m_atlasOffset->Set(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));
	}

	void Gui::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		if (!IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosGuis::UboObject uboObject = {};
		uboObject.transform = Vector4(*GetScreenTransform());
		uboObject.colourOffset = Colour(*m_colourOffset);
		uboObject.atlasOffset = Vector2(*m_atlasOffset);
		uboObject.atlasRows = static_cast<float>(m_texture->GetNumberOfRows());
		uboObject.alpha = GetAlpha();
		m_uniformObject->Update(&uboObject);

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_x);
		scissorRect.offset.y = static_cast<uint32_t>(Display::Get()->GetHeight() * -GetScissor()->m_y);
		scissorRect.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_z);
		scissorRect.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_w);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			m_uniformObject->GetWriteDescriptor(0, *descriptorSet),
			m_texture->GetWriteDescriptor(1, *descriptorSet)
		};
		descriptorSet->Update(descriptorWrites);

		// Draws the object.
		descriptorSet->BindDescriptor(commandBuffer, pipeline);
		m_model->CmdRender(commandBuffer);
	}
}
