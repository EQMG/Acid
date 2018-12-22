#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Handlers/StorageHandler.hpp"

namespace acid
{
	class RendererFonts2 : // ACID_EXPORT
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformScene;
		StorageHandler m_storageInstances;
		StorageHandler m_storageCells;
		StorageHandler m_storagePoints;
	public:
		explicit RendererFonts2(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;
	};
}
