#include "RendererShadows.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Scenes/Scenes.hpp"
#include "ShadowRender.hpp"
#include "UbosShadows.hpp"

namespace Flounder
{
	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosShadows::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Shadows/Shadow.vert", "Resources/Shaders/Shadows/Shadow.frag"},
			VertexModel::GetBindingDescriptions(), PIPELINE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT), { }))
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererShadows::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosShadows::UboScene uboScene = {};
		uboScene.projectionView = *Shadows::Get()->GetShadowBox()->GetProjectionViewMatrix();
		uboScene.cameraPosition = *Scenes::Get()->GetCamera()->GetPosition();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<ShadowRender *> renderList = std::vector<ShadowRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>(&renderList);

		for (auto shadowRender : renderList)
		{
			shadowRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
