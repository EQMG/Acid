#include "TestEntity.hpp"

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "ShaderTest.hpp"

namespace Flounder
{
	TestEntity::TestEntity(const Vector3 &position, const Vector3 &rotation, Model *model, Texture *diffuse, Texture *swapMap) :
		m_uniformObject(new UniformBuffer(sizeof(ShaderTest::UboObject))),
		m_model(model),
		m_diffuse(diffuse),
		m_swapMap(swapMap),
		m_position(position),
		m_rotation(rotation)
	{
	}

	TestEntity::~TestEntity()
	{
		delete m_swapMap;
		delete m_diffuse;
		delete m_model;
		delete m_uniformObject;
	}

	void TestEntity::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_diffuse->GetWriteDescriptor(2, descriptorSet) , m_swapMap->GetWriteDescriptor(3, descriptorSet) }; // TODO: Modulaize this! , m_texture->GetWriteDescriptor(2, descriptorSet)

		ShaderTest::UboObject uboObject = {};
		uboObject.transform = Matrix4();
		uboObject.swaying = 1.0f;
		const float swayPower = 0.15f;
		const float wx = sin(m_position.m_x * 0.6f);
		const float wz = sin(m_position.m_z * 0.5f);
		const float sx = fabs(wx) + fabs(wz);
		const float st = Engine::Get()->GetTime() * sx;
		const float swayX = swayPower * (sin(0.25f * st) - sin(1.2f * st) + cos(0.5f * st));
		const float swayY = swayPower * (cos(0.25f * st) - cos(1.2f * st) + sin(0.5f * st));
		uboObject.swayOffset = Vector2(swayX, swayY);
		Matrix4::TransformationMatrix(m_position, m_rotation, 1.0f, &uboObject.transform);
		m_uniformObject->Update(&uboObject);

		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		m_model->CmdRender(commandBuffer);
	}
}
