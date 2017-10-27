#include "Gui.hpp"

#include "../devices/Display.hpp"
#include "UbosGuis.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES = {
		Vertex(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(0.5f, 0.5f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES = {
		0, 1, 2, 2, 3, 0
	};

	Gui::Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const Vector2 &pivot, const bool &inScreenCoords, Texture *texture, const int &selectedRow) :
		UiObject(parent, position, dimensions, pivot, inScreenCoords),
		m_uniformObject(new UniformBuffer(sizeof(UbosGuis::UboObject))),
		m_model(new Model(VERTICES, INDICES)),
		m_texture(texture),
		m_selectedRow(selectedRow),
		m_textureOffset(new Vector2()),
		m_colourOffset(new Colour())
	{
	}

	Gui::~Gui()
	{
		delete m_uniformObject;
		delete m_model;
		delete m_textureOffset;
		delete m_colourOffset;
	}

	void Gui::UpdateObject()
	{
		const int numberOfRows = m_texture != nullptr ? m_texture->GetNumberOfRows() : 1;
		const int column = m_selectedRow % numberOfRows;
		const int row = m_selectedRow / numberOfRows;
		m_textureOffset->Set(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));
	}

	void Gui::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		if (!IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosGuis::UboObject uboObject = {};
		uboObject.scissor = Vector4(*GetScissor());
		uboObject.transform = Vector4(
			GetScreenPosition()->m_x, GetScreenPosition()->m_y,
			GetScreenDimensions()->m_x, GetScreenDimensions()->m_y
		);
		uboObject.atlasRows = static_cast<float>(m_texture->GetNumberOfRows());
		uboObject.atlasOffset = Vector2(*m_textureOffset);
		uboObject.colourOffset = Colour(*m_colourOffset);
		uboObject.alpha = GetAlpha();
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformObject->GetWriteDescriptor(0, descriptorSet), m_texture->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
