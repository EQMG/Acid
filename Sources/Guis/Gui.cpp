#include "Gui.hpp"

#include "Display/Display.hpp"
#include "Models/Shapes/ModelRectangle.hpp"

namespace acid
{
	Gui::Gui(UiObject *parent, const UiBound &rectangle, const std::shared_ptr<Texture> &texture) :
		UiObject(parent, rectangle),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler()),
		m_model(ModelRectangle::Resource(0.0f, 1.0f)),
		m_texture(texture),
		m_numberOfRows(1),
		m_selectedRow(0),
		m_atlasOffset(Vector2()),
		m_colourOffset(Colour(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	void Gui::UpdateObject()
	{
		int32_t numberOfRows = m_texture != nullptr ? m_numberOfRows : 1;
		int32_t column = m_selectedRow % numberOfRows;
		int32_t row = m_selectedRow / numberOfRows;
		m_atlasOffset = Vector2(static_cast<float>(column) / static_cast<float>(numberOfRows), static_cast<float>(row) / static_cast<float>(numberOfRows));

		// Updates uniforms.
		m_uniformObject.Push("modelMatrix", GetModelMatrix());
		m_uniformObject.Push("screenOffset", GetScreenTransform());
		m_uniformObject.Push("colourOffset", m_colourOffset);
		m_uniformObject.Push("atlasOffset", m_atlasOffset);
		m_uniformObject.Push("atlasRows", static_cast<float>(m_numberOfRows));
		m_uniformObject.Push("alpha", GetAlpha());
		m_uniformObject.Push("depth", GetDepth());
		m_uniformObject.Push("modelMode", GetWorldTransform() ? (IsLockRotation() + 1) : 0);
	}

	bool Gui::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene)
	{
		// Gets if this should be rendered.
		if (!IsVisible() || GetAlpha() == 0.0f)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		m_descriptorSet.Push("samplerColour", m_texture);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<int32_t>(pipeline.GetWidth() * GetScissor().m_x);
		scissorRect.offset.y = static_cast<int32_t>(pipeline.GetHeight() * GetScissor().m_y);
		scissorRect.extent.width = static_cast<uint32_t>(pipeline.GetWidth() * GetScissor().m_z);
		scissorRect.extent.height = static_cast<uint32_t>(pipeline.GetHeight() * GetScissor().m_w);
		vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
		return true;
	}
}
