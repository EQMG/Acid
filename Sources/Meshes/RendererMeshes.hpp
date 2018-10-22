#pragma once

#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	enum MeshSort
	{
		MESH_SORT_NONE = 0,
		MESH_SORT_FRONT = 1,
		MESH_SORT_BACK = 2
	};

	class ACID_EXPORT RendererMeshes :
		public IRenderer
	{
	private:
		MeshSort m_meshSort;
		UniformHandler m_uniformScene;
	public:
		explicit RendererMeshes(const GraphicsStage &graphicsStage, const MeshSort &meshSort = MESH_SORT_NONE);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
