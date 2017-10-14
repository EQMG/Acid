#include "Skybox.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "UbosSkyboxes.hpp"

namespace Flounder
{
	Skybox::Skybox(Cubemap *cubemap, Model *model, const float &size) :
		m_uniformObject(new UniformBuffer(sizeof(UbosSkyboxes::UboObject))),
		m_cubemap(cubemap),
		m_model(model),
		m_size(size),
		m_blend(1.0f),
		m_rotation(new Vector3()),
		m_modelMatrix(new Matrix4())
	{
	}

	Skybox::~Skybox()
	{
		delete m_cubemap;
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

		UbosSkyboxes::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.skyColour = Colour("#366996");
		uboObject.blendFactor = m_blend;
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_cubemap->GetWriteDescriptor(2, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
