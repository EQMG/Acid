#include "TestEntity.hpp"

#include "../engine/Engine.hpp"
#include "TestShader.hpp"

namespace Flounder
{
	TestEntity::TestEntity(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(TestShader::UboObject))),
		m_model(new Model("res/treeBirchSmall/model.obj")),
		m_diffuse(new Texture("res/treeBirchSmall/diffuse.png")),
		m_swapMap(new Texture("res/treeBirchSmall/sway.png")),

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

	void TestEntity::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		TestShader::UboObject uboObject= {};
		uboObject.model = Matrix4();
		uboObject.swaying = 1.0f;
		const float swayPower = 0.15f;
		const float wx = sin(m_position.m_x * 0.6f);
		const float wz = sin(m_position.m_z * 0.5f);
		const float sx = fabs(wx) + fabs(wz);
		const float st = Engine::Get()->GetTime() * sx;
		const float swayX = swayPower * (sin(0.25f * st) - sin(1.2f * st) + cos(0.5f * st));
		const float swayY = swayPower * (cos(0.25f * st) - cos(1.2f * st) + sin(0.5f * st));
		uboObject.swayOffset = Vector2(swayX, swayY);
		Matrix4::TransformationMatrix(m_position, m_rotation, 1.0f, &uboObject.model);
		m_uniformObject->Update(&uboObject);

		VkBuffer vertexBuffers[] = { m_model->GetVertexBuffer()->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexBuffer()->GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_model->GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
	}
}
