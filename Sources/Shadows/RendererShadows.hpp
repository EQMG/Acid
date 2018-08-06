#pragma once

#include "Models/Model.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Shadows.hpp"

namespace acid
{
	class ACID_EXPORT RendererShadows :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		RendererShadows(const GraphicsStage &graphicsStage);

		~RendererShadows();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
