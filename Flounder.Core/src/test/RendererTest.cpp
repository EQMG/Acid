#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "../renderer/Renderer.hpp"
#include "../terrains/Terrains.hpp"
#include "UbosTest.hpp"

namespace Flounder
{
	const DescriptorType RendererTest::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTest::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererTest::typeSamplerDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererTest::typeSamplerSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	const PipelineCreateInfo RendererTest::pipelineCreateInfo =
	{
		PIPELINE_POLYGON, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags
		InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions()), // inputState
		Descriptor::Create({ typeUboScene, typeUboObject, typeSamplerDiffuse, typeSamplerSway }), // descriptor
		ShaderStages::Create({ ShaderStage("res/shaders/tests/test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT), ShaderStage("res/shaders/tests/test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT) }) // shaderStages
	};

	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTest::UboScene))),
		m_pipeline(new Pipeline("tests", pipelineCreateInfo)),
		m_testEntities(std::vector<TestEntity*>())
	{
		/*Model *model = new Model("res/treeBirchSmall/model.obj");
		Texture *diffuse = new Texture("res/treeBirchSmall/diffuse.png"); // "res/undefined.png"
		Texture *swapMap = new Texture("res/treeBirchSmall/sway.png");

		for (int i = -5; i <= 5; i++)
		{
			for (int j = -5; j <= 5; j++)
			{
				const float xv = Maths::RandomInRange(-10.0f, 10.0f);
				const float yv = Maths::RandomInRange(-10.0f, 10.0f);
				m_testEntities.push_back(new TestEntity(Terrains::Get()->GetPosition((20.0f * i) + xv, (20.0f * j) + yv), Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), model, diffuse, swapMap));
			}
		}*/
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
