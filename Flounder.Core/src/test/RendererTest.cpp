#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "../renderer/Renderer.hpp"
#include "../terrains/Terrains.hpp"
#include "UbosTest.hpp"
#include "../camera/Camera.hpp"

namespace Flounder
{
	const DescriptorType RendererTest::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTest::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTest::typeSamplerDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererTest::typeSamplerSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	const PipelineCreateInfo RendererTest::pipelineCreateInfo =
	{
		PIPELINE_MRT, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags
		0, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerDiffuse, typeSamplerSway }, // descriptors

		{ "res/shaders/tests/test.vert.spv", "res/shaders/tests/test.frag.spv" } // shaderStages
	};

	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTest::UboScene))),
		m_pipeline(new Pipeline("tests", pipelineCreateInfo)),
		m_testEntities(std::vector<TestEntity*>())
	{
		Model *model = new Model("res/entities/cuboid/model.obj");
		Texture *diffuse = new Texture("res/entities/cuboid/diffuse.png"); // "res/undefined.png"
		Texture *swapMap = new Texture("res/entities/cuboid/sway.png");

		for (int i = 0; i <= 0; i++) // int i = -1; i <= 1; i++
		{
			for (int j = 0; j <= 0; j++) // int j = -1; j <= 1; j++
			{
				const float xv = 0.0f; // Maths::RandomInRange(-10.0f, 10.0f);
				const float yv = 0.0f; // Maths::RandomInRange(-10.0f, 10.0f);
				
				Vector3 position = Terrains::Get()->GetPosition((20.0f * i) + xv, (20.0f * j) + yv);
				position.m_y += 1.0f;
				
				if (position.m_y > 0.0f)
				{
					m_testEntities.push_back(new TestEntity(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), model, diffuse, swapMap));
				}
			}
		}
	}

	RendererTest::~RendererTest()
	{
		delete m_uniformScene;
		delete m_pipeline;

		for (auto entity : m_testEntities)
		{
			delete entity;
		}
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosTest::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto entity : m_testEntities)
		{
			entity->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
