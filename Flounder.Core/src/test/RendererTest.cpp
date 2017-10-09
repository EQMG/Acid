#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "../renderer/Renderer.hpp"
#include "../terrains/Terrains.hpp"
#include "ShaderTest.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(ShaderTest::UboScene))),
		m_testEntities(std::vector<TestEntity*>()),
		m_shader(new Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(new Pipeline("tests", m_shader, ShaderTest::pipelineCreateInfo, ShaderTest::inputState, ShaderTest::descriptor))
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

		for (auto entity : m_testEntities)
		{
			delete entity;
		}

		delete m_shader;
		delete m_pipeline;
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		ShaderTest::UboScene uboScene = {};
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
