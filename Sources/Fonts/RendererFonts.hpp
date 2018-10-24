#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Text.hpp"

namespace acid
{
	class ACID_EXPORT RendererFonts :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		explicit RendererFonts(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
