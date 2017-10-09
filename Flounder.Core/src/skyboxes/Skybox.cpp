#include "Skybox.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "ShaderSkyboxes.hpp"

namespace Flounder
{
	Skybox::Skybox(Texture *texture, Model *model, const float &size) :
		m_uniformObject(new UniformBuffer(sizeof(ShaderSkyboxes::UboObject))),
		m_texture(texture),
		m_model(model),
		m_size(size),
		m_blend(1.0f),
		m_rotation(new Vector3()),
		m_modelMatrix(new Matrix4())
	{
	}

	Skybox::~Skybox()
	{
		delete m_texture;
		delete m_model;

		delete m_rotation;
		delete m_modelMatrix;
	}

	void Skybox::Update()
	{
		Matrix4::TransformationMatrix(*Camera::Get()->GetCamera()->GetPosition(), *m_rotation, m_size, m_modelMatrix);
	}

	void Skybox::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this! , m_texture->GetWriteDescriptor(2, descriptorSet)

		ShaderSkyboxes::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.skyColour = Colour("#366996");
		uboObject.blendFactor = m_blend;
		m_uniformObject->Update(&uboObject);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkBuffer vertexBuffers[] = { m_model->GetVertexBuffer()->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexBuffer()->GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_model->GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
	}
}
