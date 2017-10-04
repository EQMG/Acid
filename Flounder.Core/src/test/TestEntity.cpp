#include "TestEntity.hpp"

#include "TestUbos.hpp"
#include "../engine/Engine.hpp"

namespace Flounder
{
	TestEntity::TestEntity() :
		m_uniformObject(UniformBuffer(sizeof(UboObject), VK_SHADER_STAGE_VERTEX_BIT)),
		m_model(Model("res/treeBirchSmall/model.obj")),
		m_diffuse(Texture("res/treeBirchSmall/diffuse.png", VK_SHADER_STAGE_FRAGMENT_BIT)),
		m_swapMap(Texture("res/treeBirchSmall/sway.png", VK_SHADER_STAGE_VERTEX_BIT))
	{
	}

	TestEntity::~TestEntity()
	{
	}

	void TestEntity::Create()
	{
		m_uniformObject.Create();
		m_model.Create();
		m_diffuse.Create();
		m_swapMap.Create();
	}

	void TestEntity::Cleanup()
	{
		m_swapMap.Cleanup();
		m_diffuse.Cleanup();
		m_model.Cleanup();
		m_uniformObject.Cleanup();
	}

	void TestEntity::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		UboObject uboObject = {};
		uboObject.model = Matrix4();
		uboObject.swaying = 1.0f;
		const float systemTime = Engine::Get()->GetTime();
		const float swayX = 0.24f * (sin(0.25f * systemTime) - sin(1.2f * systemTime) + cos(0.5f * systemTime));
		const float swayY = 0.24f * (cos(0.25f * systemTime) - cos(1.2f * systemTime) + sin(0.5f * systemTime));
		uboObject.swayOffset = Vector2(swayX, swayY);
		Matrix4::TransformationMatrix(Vector3(0.0f, -2.3f, 3.0f), Vector3(), 1.0f, &uboObject.model);
		m_uniformObject.Update(&uboObject);

		VkBuffer vertexBuffers[] = { m_model.GetVertexBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_model.GetIndexBuffer().GetBuffer(), 0, m_model.GetIndexBuffer().GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_model.GetIndexBuffer().GetIndexCount(), 1, 0, 0, 0);
	}
}
