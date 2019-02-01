#include "Gui.hpp"
#include "Gui.hpp"

#include "Renderer/Renderer.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Maths/Visual/DriverConstant.hpp"

namespace acid
{
	Gui::Gui(UiObject *parent, const UiBound &rectangle, const std::shared_ptr<Texture> &texture, const Colour &colourOffset) :
		UiObject(parent, rectangle),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler()),
		m_model(ModelRectangle::Create(0.0f, 1.0f)),
		m_texture(texture),
		m_numberOfRows(1),
		m_selectedRow(0),
		m_atlasOffset(Vector2::Zero),
		m_ninePatches(Vector4::Zero),
		m_colourDriver(std::make_unique<DriverConstant<Colour>>(colourOffset)),
		m_colourOffset(Colour())
	{
	}

	void Gui::UpdateObject()
	{
		int32_t numberOfRows = m_texture != nullptr ? m_numberOfRows : 1;
		int32_t column = m_selectedRow % numberOfRows;
		int32_t row = m_selectedRow / numberOfRows;
		m_atlasOffset = Vector2(static_cast<float>(column) / static_cast<float>(numberOfRows), static_cast<float>(row) / static_cast<float>(numberOfRows));

		m_colourOffset = m_colourDriver->Update(Engine::Get()->GetDelta());

		// Updates uniforms.
		m_uniformObject.Push("aspectRatio", Window::Get()->GetAspectRatio());
		m_uniformObject.Push("modelMatrix", GetModelMatrix());
		m_uniformObject.Push("screenOffset", Vector4(2.0f * GetScreenDimensions(), 2.0f * GetScreenPosition() - 1.0f));
		m_uniformObject.Push("modelMode", GetWorldTransform() ? (IsLockRotation() + 1) : 0);
		m_uniformObject.Push("depth", GetScreenDepth());
		m_uniformObject.Push("alpha", GetScreenAlpha());

		m_uniformObject.Push("colourOffset", m_colourOffset);
		m_uniformObject.Push("atlasOffset", m_atlasOffset);
		m_uniformObject.Push("atlasRows", static_cast<float>(m_numberOfRows));
		m_uniformObject.Push("ninePatches", m_ninePatches);
	}

	bool Gui::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
	{
		// Gets if this should be rendered.
		if (m_texture == nullptr || !IsEnabled())
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
		scissorRect.offset.x = static_cast<int32_t>(pipeline.GetWidth() * GetScissor().m_x) + 1;
		scissorRect.offset.y = static_cast<int32_t>(pipeline.GetHeight() * GetScissor().m_y) + 1;
		scissorRect.extent.width = static_cast<uint32_t>(pipeline.GetWidth() * GetScissor().m_z);
		scissorRect.extent.height = static_cast<uint32_t>(pipeline.GetHeight() * GetScissor().m_w);
		vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
		m_model->CmdRender(commandBuffer);
		return true;
	}
}
