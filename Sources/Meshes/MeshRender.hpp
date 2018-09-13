#pragma once

#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Mesh.hpp"

namespace acid
{
	class ACID_EXPORT MeshRender :
		public IComponent
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;
	public:
		MeshRender();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		bool CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const GraphicsStage &graphicsStage);

		bool operator<(const MeshRender &other) const;
	};
}
