#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Textures/Cubemap.hpp"

namespace fl
{
	class FL_EXPORT RendererDeferred :
		public IRenderer
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformScene;

		Pipeline *m_pipeline;
		Model *m_model;

		Texture *m_brdflut;
	public:
		RendererDeferred(const GraphicsStage &graphicsStage);

		~RendererDeferred();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
