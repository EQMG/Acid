#include "Gui.hpp"

#include "../devices/Display.hpp"
#include "UbosGuis.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES = {
		Vertex(Vector3(0.0f, 0.0f, 0.0f)),
		Vertex(Vector3(0.0f, 1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 0.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f))
	};

	Gui::Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, Texture *texture, const int &selectedRow) :
		UiObject(parent, position, dimensions),
		m_uniformObject(new UniformBuffer(sizeof(UbosGuis::UboObject))),
		m_model(new Model(VERTICES)),
		m_texture(texture),
		m_flipTexture(false),
		m_selectedRow(selectedRow),
		m_textureOffset(new Vector2()),
		m_colourOffset(new Colour())
	{
		SetMeshSize(Vector2(0.5f, 0.5f));
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

	void Gui::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosGuis::UboObject uboObject = {};
		uboObject.scissor = Vector4(*GetScissor());
		uboObject.size = Vector2(*GetMeshSize());
		uboObject.transform = Vector4(GetScreenPosition()->m_x, GetScreenPosition()->m_y,
			GetScreenDimensions()->m_x, GetScreenDimensions()->m_y);
		uboObject.rotation = Maths::Radians(GetRotation());
		uboObject.alpha = GetAlpha();
		uboObject.flipTexture = static_cast<float>(m_flipTexture);
		uboObject.atlasRows = static_cast<float>(m_texture->GetNumberOfRows());
		uboObject.atlasOffset = Vector2(*m_textureOffset);
		uboObject.colourOffset = Colour(*m_colourOffset);
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_texture->GetWriteDescriptor(2, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
